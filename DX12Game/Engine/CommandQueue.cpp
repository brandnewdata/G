#include "pch.h"
#include "CommandQueue.h"


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
	// Observe that for CreateCommandList, we specify null for the pipeline state object
	// parameter.In this chapter’s sample program, we do not issue any draw commands, so we
	// do not need a valid pipeline state object.We will discuss pipeline state objects in Chapter6.
	hResult = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get() , // Associated command allocator
		nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&_cmdList);

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	hResult = _cmdList->Close();
}
