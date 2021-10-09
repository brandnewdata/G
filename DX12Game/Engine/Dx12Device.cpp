#include "pch.h"
#include "Dx12Device.h"

void Dx12Device::Init()
{
#ifdef _DEBUG
	// ����׿�
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
#endif
	// ���丮 ���� (������ ���� ���丮 ����)
	// @param1 rrid(����̽��� COM ID)
	// @param2 ppDevice(�ּ�)
	// IID_PPV_ARGS ��ũ�� ���� ����
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

	// @param1 pAdapter : nullptr �����ϸ� �ý��� �⺻ ���÷��� �����
	// @param2 MinimumFeatureLevel : ���� ���α׷��� �䱸�ϴ� �ּ� ��� ����, �뷫 DirectX 11�̻� �����ϴ� �׷��� ī���� ����...
	// @param3 riid : Device�� COM ID
	// @param4 ppDevice : ������ ��ġ�� �����͸� ���� ����
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
}