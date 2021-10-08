#pragma once
#include "pch.h"

// direct x �� gpu�� �����ϰ� ���α׷����� �� ���̴� ������ �׷��� API
// COM(Component Object Model)
// - DX�� ���α׷��� ��� �������� ���� ȣȯ���� �����ϰ� �ϴ� ���
// - COM ��ü(COM �������̽�)�� ���. ���λ����� �츮���� ������
// - ComPtr ������ ����Ʈ ������
// - GPU ����ȭ�� ��ü�� �����ϴ� ������ ComPtr�� �����

// https://docs.microsoft.com/ko-kr/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi
// DXGI(DirectX Graphics Infrastructure) �Ʒ��� ��ǥ���� ���ó��.
// @ ��üȭ�� ��� ��ȯ
// @ �����Ǵ� ���÷��� ��� Ȯ��
// @ ���� ü�� ����
// @ ���ø����̼ǰ� Ŀ�� ���̿��� ������ ó���� �����ϰ� �ϴ� API
// @ Device�� �����ϱ� ���ؼ��� IDXGIFactory��� COM��ü�� �ʿ��ϴ�.

// IDXGIFactory
// @ �����(���� ī��), �����ġ(�����), ����̽�(�׷���ī��)�� �����ϴ� COM��ü

// Device
// �׷��� ī�带 ����ȭ�� ��ü(�ٸ� ���� �����)

//--------------------------------------------------------------------------------------------------------------------------------------------------//

class Dx12Device
{
public:
	void Init();
	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }
private:
	ComPtr<ID3D12Debug>		_debugController;
	ComPtr<IDXGIFactory>	_dxgi; // ȭ�� ���� ��ɵ� (���..)
	ComPtr<ID3D12Device>	_device; // ���� ��ü ����
};
