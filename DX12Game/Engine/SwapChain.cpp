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
	sd.BufferDesc.Width = static_cast<uint32>(info.width);					// ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<uint32>(info.height);				// ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60;								// ȭ�� ���� ����
	sd.BufferDesc.RefreshRate.Denominator = 1;								// ȭ�� ���� ����
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;						// ������ ���÷��� ����  8��Ʈ�� 4�� 32��Ʈ
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;												// ��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						// �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;								// ����+�ĸ� ���� ���� ������ �۾��ؾ���
	sd.OutputWindow = info.hWnd;
	sd.Windowed = info.bWindowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;							// ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// dxgi ��°� ���õ� �͵��� �� ���ִ�.
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_SwapChain);		// ������ spec���� SwapChain ����.

	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_SwapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i])); // ���۸� rendertargets�� ����
}

void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// RTV�� DescHeap Size�� ���´�.
	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // SwapChain class ����ο� �� ���̴�.
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;  // flag ����.
	rtvDesc.NodeMask = 0;

	// ���� DescHeap�� �����ϴ� �޸� ���� �迭�� ��� ������. (RTV, DSV, CBV ���� �迭�� ������)
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvDescHeap));

	// RTV ���ҽ� �޸� �������� ���´�.
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvDescHeap->GetCPUDescriptorHandleForHeapStart();

	for(int i =0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// ���ҽ� �޸� �������� �޸� ũ�� ������ ������ �ڵ��� ������.
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		// �ڵ�� ���� Ÿ�ٺ並 �����ϰ� �� �ּҸ� �迭�� �޾ƿ´�.
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);
	}
}

void SwapChain::SwapIndex()
{
	// ���� �ε����� ���´�. ���簡 ������ �ε����� 0�� �ε����� ���ư���.
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

void SwapChain::Present()
{
	_SwapChain->Present(0, 0);
}


