#pragma once
#include "pch.h"

// direct x 는 gpu를 제어하고 프로그래밍할 때 쓰이는 저수준 그래픽 API
// COM(Component Object Model)
// - DX의 프로그래밍 언어 독립성과 하위 호환성을 가능하게 하는 기술
// - COM 객체(COM 인터페이스)를 사용. 세부사항은 우리한테 숨겨짐
// - ComPtr 일종의 스마트 포인터
// - GPU 가상화한 객체에 접근하는 도구로 ComPtr을 사용함

// https://docs.microsoft.com/ko-kr/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi
// DXGI(DirectX Graphics Infrastructure) 아래는 대표적인 사용처들.
// @ 전체화면 모드 전환
// @ 지원되는 디스플레이 모드 확인
// @ 스왑 체인 생성
// @ 어플리케이션과 커널 사이에서 저수준 처리를 가능하게 하는 API
// @ Device를 생성하기 위해서는 IDXGIFactory라는 COM객체가 필요하다.

// IDXGIFactory
// @ 어댑터(비디오 카드), 출력장치(모니터), 디바이스(그래픽카드)를 포함하는 COM객체

// Device
// 그래픽 카드를 가상화한 객체(다른 말로 어댑터)

//--------------------------------------------------------------------------------------------------------------------------------------------------//

class Dx12Device
{
public:
	void Init();
	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }
private:
	ComPtr<ID3D12Debug>		_debugController;
	ComPtr<IDXGIFactory>	_dxgi; // 화면 관련 기능들 (출력..)
	ComPtr<ID3D12Device>	_device; // 각종 객체 생성
};
