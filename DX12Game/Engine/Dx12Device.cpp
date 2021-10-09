#include "pch.h"
#include "Dx12Device.h"

void Dx12Device::Init()
{
#ifdef _DEBUG
	// 디버그용
	::D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
	_debugController->EnableDebugLayer();
#endif
	// 팩토리 생성 (디자인 패턴 팩토리 참고)
	// @param1 rrid(디바이스의 COM ID)
	// @param2 ppDevice(주소)
	// IID_PPV_ARGS 매크로 선언 참조
	::CreateDXGIFactory(IID_PPV_ARGS(&_dxgi));

	// @param1 pAdapter : nullptr 지정하면 시스템 기본 디스플레이 어댑터
	// @param2 MinimumFeatureLevel : 응용 프로그램이 요구하는 최소 기능 수준, 대략 DirectX 11이상 지원하는 그래픽 카드라고 이해...
	// @param3 riid : Device의 COM ID
	// @param4 ppDevice : 생성된 장치의 포인터를 받을 변수
	::D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_device));
}