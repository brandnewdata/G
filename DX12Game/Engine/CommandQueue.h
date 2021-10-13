#pragma once
#include "pch.h"

// @@ CommandQueue // DX12 부터 새로 생김.  
// @ Queue COM객체
// @ CommandList들을 GPU에 전달하는 목적으로 사용함.
// @ Queue에 명령을 쌓았다가 적절한 시점에 ExcuteCommandList() 함수로 명령을 전달
// @ 명령 예약 방식이라서 즉시 실행아님.

class CommandQueue
{
	// 초기화 함수
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device);
// 렌더링 관리 함수들
public:
	void WaitSync();
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	// getter, setter
public:
	ComPtr<ID3D12CommandQueue>			GetCmdQueue		() { return _cmdQueue;			}

private:
	// @ 명령 큐, 명령 할당자, 명령 리스트 포인터
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	// 펜스 포인터, 펜스 값, 이벤트 핸들 // 멀티스레드에서 사용하는 mutex같은 동기화 객체와 유사한 역할
	ComPtr<ID3D12Fence>					_Fence;
	uint32								_nFenceValue;
	HANDLE								_hFenceEvent;

	shared_ptr<class SwapChain>			_swapChain;
};
