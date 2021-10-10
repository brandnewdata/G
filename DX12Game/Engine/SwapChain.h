#pragma once
#include "pch.h"

// @ ID3D12Resource, @ DescHeap 렌더 타겟이든지 텍스쳐든지 GPU에서 사용할 메모리덩어리(Resource)에 대한 스펙을 묘사한 구조체입니다.
// DX11 View == DX12 Descriptor
// DX11의 RTV(RenderTargetVeiw), DSV(DepthStencilView), CBV(ConstantBufferView), UAV(UnorderedAccessView) 전부
// DX12에서는 DescHeap에 입력한 스펙대로 사용할 메모리를 배정받음.
// 어쩌면 이 부분은 덜 복잡해진 것 같기도 함.
//
// @ 더블 버퍼링과 @ Swap Chain
// 렌더링용 버퍼는 n개 존재 할 수있습니다. 하지만 성능상의 이유로 보통 2개만 써서 더블 버퍼링을 합니다.
// @ Back Buffer  : DescHeap에 설정된 스펙에 따라서 렌더링중인 버퍼.
// @ Front Buffer : 출력 처리에 사용중인 버퍼 그래픽 카드에서만 사용한다. 버퍼의 내용을 그대로 모니터에 출력한다.
// @ Presentation : Back Buffer에 렌더링이 끝나면 Front Buffer에서 Back Buffer로 바꿉니다.
// 한마디로 인데스 0번 1번이 있다면 서로 계속 돌아가면서 모니터에 출력 합니다.

class SwapChain
{
	// 초기화
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void SwapIndex();			// BackBuffer 인덱스를 현재 다음 인덱스로 만든다. 현재가 마지막 인덱스면 0번으로 만든다.
	void Present();				// BackBuffer와 FrontBuffer swap!

// class 내부에서만 접근하는 속성들을 초기화
private:
	void CreateSwapChain(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);	// RenderTargetVeiw를 초기화 합니다.

// Getter, Setter
public:
	ComPtr<IDXGISwapChain> GetSwapChain()								const { return _swapChain; }

	ComPtr<ID3D12Resource> GetRTVBufferByIndex(const uint32 rtvIndex)	const { return _rtvBuffer[rtvIndex]; }

	ComPtr<ID3D12Resource> GetBackRTV_Buffer()							const { return _rtvBuffer[_backBufferIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV_Handle()						const { return _rtvHandle[_backBufferIndex]; }

private:
	ComPtr<IDXGISwapChain>			_swapChain;

	ComPtr<ID3D12Resource>			_rtvBuffer[ECoreSetting::SWAP_CHAIN_BUFFER_COUNT];	// 렌더 타겟으로 사용할 버퍼 주소배열
	ComPtr<ID3D12DescriptorHeap>	_rtvDescHeap;							// 추후 설명 추가
	// cpu에 대한 구조체가 아니라 // cpu측의 rtv Desc에 대한 핸들으로 보인다.
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[ECoreSetting::SWAP_CHAIN_BUFFER_COUNT];

	uint32							_backBufferIndex = 0;					// 렌더링 대상 버퍼의 인덱스
};

