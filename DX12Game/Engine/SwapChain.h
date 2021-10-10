#pragma once

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
	ComPtr<IDXGISwapChain> GetSwapChain()								const { return _SwapChain; }

	ComPtr<ID3D12Resource> GetRTVBufferByIndex(const uint32 rtvIndex)	const { return _rtvBuffer[rtvIndex]; }

	ComPtr<ID3D12Resource> GetBackRTV_Buffer()							const { return _rtvBuffer[_backBufferIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV_Handle()						const { return _rtvHandle[_backBufferIndex]; }

// 내부에서만 사용하는 const들.
private:
	static constexpr uint32 SWAP_CHAIN_BUFFER_COUNT = 2; // 더블 버퍼링을 할 것 이다!

private:
	ComPtr<IDXGISwapChain>			_SwapChain;

	ComPtr<ID3D12Resource>			_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];	// 렌더 타겟으로 사용할 버퍼 주소배열
	ComPtr<ID3D12DescriptorHeap>	_rtvDescHeap;							// 추후 설명 추가
	// cpu에 대한 구조체가 아니라 // cpu측의 rtv Desc에 대한 핸들으로 보인다.
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	uint32							_backBufferIndex = 0;					// 렌더링 대상 버퍼의 인덱스
};

