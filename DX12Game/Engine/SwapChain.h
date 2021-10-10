#pragma once
#include "pch.h"

// @ ID3D12Resource, @ DescHeap ���� Ÿ���̵��� �ؽ��ĵ��� GPU���� ����� �޸𸮵��(Resource)�� ���� ������ ������ ����ü�Դϴ�.
// DX11 View == DX12 Descriptor
// DX11�� RTV(RenderTargetVeiw), DSV(DepthStencilView), CBV(ConstantBufferView), UAV(UnorderedAccessView) ����
// DX12������ DescHeap�� �Է��� ������ ����� �޸𸮸� ��������.
// ��¼�� �� �κ��� �� �������� �� ���⵵ ��.
//
// @ ���� ���۸��� @ Swap Chain
// �������� ���۴� n�� ���� �� ���ֽ��ϴ�. ������ ���ɻ��� ������ ���� 2���� �Ἥ ���� ���۸��� �մϴ�.
// @ Back Buffer  : DescHeap�� ������ ���忡 ���� ���������� ����.
// @ Front Buffer : ��� ó���� ������� ���� �׷��� ī�忡���� ����Ѵ�. ������ ������ �״�� ����Ϳ� ����Ѵ�.
// @ Presentation : Back Buffer�� �������� ������ Front Buffer���� Back Buffer�� �ٲߴϴ�.
// �Ѹ���� �ε��� 0�� 1���� �ִٸ� ���� ��� ���ư��鼭 ����Ϳ� ��� �մϴ�.

class SwapChain
{
	// �ʱ�ȭ
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void SwapIndex();			// BackBuffer �ε����� ���� ���� �ε����� �����. ���簡 ������ �ε����� 0������ �����.
	void Present();				// BackBuffer�� FrontBuffer swap!

// class ���ο����� �����ϴ� �Ӽ����� �ʱ�ȭ
private:
	void CreateSwapChain(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);	// RenderTargetVeiw�� �ʱ�ȭ �մϴ�.

// Getter, Setter
public:
	ComPtr<IDXGISwapChain> GetSwapChain()								const { return _swapChain; }

	ComPtr<ID3D12Resource> GetRTVBufferByIndex(const uint32 rtvIndex)	const { return _rtvBuffer[rtvIndex]; }

	ComPtr<ID3D12Resource> GetBackRTV_Buffer()							const { return _rtvBuffer[_backBufferIndex]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV_Handle()						const { return _rtvHandle[_backBufferIndex]; }

private:
	ComPtr<IDXGISwapChain>			_swapChain;

	ComPtr<ID3D12Resource>			_rtvBuffer[ECoreSetting::SWAP_CHAIN_BUFFER_COUNT];	// ���� Ÿ������ ����� ���� �ּҹ迭
	ComPtr<ID3D12DescriptorHeap>	_rtvDescHeap;							// ���� ���� �߰�
	// cpu�� ���� ����ü�� �ƴ϶� // cpu���� rtv Desc�� ���� �ڵ����� ���δ�.
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[ECoreSetting::SWAP_CHAIN_BUFFER_COUNT];

	uint32							_backBufferIndex = 0;					// ������ ��� ������ �ε���
};

