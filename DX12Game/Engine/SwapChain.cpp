#include "pch.h"
#include "SwapChain.h"

void SwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(info, device, dxgi, cmdQueue);
	CreateRTV(device);
}


void SwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	_SwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd; 
	sd.BufferDesc.Width = static_cast<uint32>(info.width);					// 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<uint32>(info.height);				// 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60;								// 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1;								// 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// 버퍼의 디스플레이 형식  8비트씩 4개 32비트
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;												// 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// 후면 버퍼에 렌더링할 것 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;								// 전면+후면 버퍼 각각 번갈아 작업해야함
	sd.OutputWindow = info.hWnd;
	sd.Windowed = info.bWindowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;							// 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// dxgi 출력과 관련된 것들이 다 들어가있다.
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_SwapChain);		// 설정한 spec으로 SwapChain 생성.

	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_SwapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i])); // 버퍼를 rendertargets에 저장
}

void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// RTV용 DescHeap Size를 얻어온다.
	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // SwapChain class 선언부에 그 값이다.
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // flag 없슴.
	rtvDesc.NodeMask = 0;

	// 같은 DescHeap을 공유하는 메모리 끼리 배열로 묶어서 관리함. (RTV, DSV, CBV 각각 배열로 관리함)
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvDescHeap));

	// RTV 리소스 메모리 시작점을 얻어온다.
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

	for(int i =0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// 리소스 메모리 시작점과 메모리 크기 정보로 윈도우 핸들을 생성함.
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		// 핸들로 렌더 타겟뷰를 생성하고 그 주소를 배열에 받아온다.
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);
	}
}

void SwapChain::SwapIndex()
{
	// 다음 인덱스를 얻어온다. 현재가 마지막 인덱스면 0번 인덱스로 돌아간다.
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::Present()
{
	_SwapChain->Present(0, 0);
}


