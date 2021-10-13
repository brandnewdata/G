#include "pch.h"
#include "CommandQueue.h"

#include "SwapChain.h"


CommandQueue::~CommandQueue()
{
}

void CommandQueue::Init(ComPtr<ID3D12Device> device)
{
	// 커맨드 큐 스펙설정 // msdn에서 초기화한 방식. c++ 최식 스펙이란 연관있음.
	D3D12_COMMAND_QUEUE_DESC CommandQueueDesc = {};
	// ::ZeroMemory(&CommandQueueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC)); // 전통적인 메모리 초기화 방식
	CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	// 커맨드 큐 생성
	HRESULT hResult = device->CreateCommandQueue(&CommandQueueDesc, IID_PPV_ARGS(&_cmdQueue));
	// 커맨드 할당자 생성
	hResult = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));
	// 커맨드 리스트 생성
	// Observe that for CreateCommandList, we specify null for the pipeline state object parameter.
	// In this chapter’s sample program, we do not issue any draw commands, so we
	// do not need a valid pipeline state object.We will discuss pipeline state objects in Chapter6. -luna dx12-
	hResult = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get() , // Associated command allocator
		nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&_cmdList);

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	hResult = _cmdList->Close();
}

void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point.
	_nFenceValue++;

	// Add an instruction to the commands up to set a new fence point.
	// Because we are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_Fence.Get(), _nFenceValue);

	// @ Returns : the current value of the fence. If the device has been removed, the return value will be UINT64_MAX.
	// fence value가 정상이라면 
	if(_Fence->GetCompletedValue() < _nFenceValue)
	{
		// Wait until the GPU hits current fence.
		// Fence event when GPU hits current fence.
		_Fence->SetEventOnCompletion(_nFenceValue, _hFenceEvent);
		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_hFenceEvent, INFINITE);
	}
	
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	// 제출할 명령 리스트를 작성한다.
	// Command list allocators can only be reset when the associated command lists have finished execution on the GPU;
	// apps should use fences to determine GPU execution progress.
	// However, when ExecuteCommandList() is called on a particular command list,
	// that command list can then be reset at any time and must be beforere-recording.
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	// 출력 중 상태인 버퍼를 렌더링 중 상태로 상태전이 시키고 그 리소스 베리어 정보를 받아온다.
	// 이미 swapchain에서는 backbuffer frontbuffer swap을 한 것으로 추정된다.
	// 이 함수는 렌더링 시작과 끝에 짝을 이뤄서 호출된다. 
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTV_Buffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // 전이 전 상태 : 출력 중 상태
		D3D12_RESOURCE_STATE_RENDER_TARGET // 전이할 상태 : 렌더링 중 상태
	);

	// 리소스 베리어 설정
	_cmdList->ResourceBarrier(1, &barrier);

	// 렌더링에 기본적으로 필요한 정보들인데 이것도 명령 리스트에 포함되어 있음.
	// Set the viewport and scissor rect. This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// 렌더링할 버퍼(리소스, 메모리...)를 지정.
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV_Handle();
	// OutMerge 타겟 설정.
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);

	// 명령들 기록
	// 렌더링 버퍼를 정리하고 가벼운 파란색으로 덮어라. (RenderBegin 전체가에 호출되는 함수는 명령리스트 작성임 바로 실행 아님.)
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
}

void CommandQueue::RenderEnd()
{
	// 렌더링 중 상태인 버퍼를 출력 중 상태로 상태전이 시키고 그 리소스 베리어 정보를 받아온다.
	// 이 함수는 렌더링 시작과 끝에 짝을 이뤄서 호출된다.
	// RenderEnd()함수에서는 버퍼에 렌더링이 끝나서 이제 출력에 넘기는 작업을 하는 것 이다.
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTV_Buffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 전이 전 상태 : 렌더링 중 상태 
		D3D12_RESOURCE_STATE_PRESENT // 전이 할 상태 : 출력 중 상태
	);

	// 리소스 베리어 설정
	_cmdList->ResourceBarrier(1, &barrier);
	// 입력완료
	_cmdList->Close();

	// 수행을 요청하는 것이다. 수행되는 시점은 모름.
	ID3D12CommandList*  cmdListArr[] = {_cmdList.Get()};
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	// 여기 이 present()의 실제 GPU실행은 이 함수가 반환한 이후가 될 수 있다는 점을 눈치채야 한다.
	// cmdQueue에 들어있는 모든 작업을 마쳐야 렌더링이 끝나고 그래야 BackBuffer와 FrontBuffer를 교체한다.
	_swapChain->Present();

	// wait until frame commands are complete.
	// This waiting is inefficient and is done for simplicity. // 비효율적이다 그리고 간결함을 위해서 수행된다. // 튜토리얼이니까. 효율보다 이해하기 편하라고...
	// Later we will show how to organize our rendering code so we do not have to wait per frame. // 나중에 매 프레임마다 안 기다려도 되게 코드 고칠거다.
	WaitSync();

	_swapChain->SwapIndex();
}
