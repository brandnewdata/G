#pragma once
#include "pch.h"

// @@ CommandQueue // DX12 부터 새로 생김.  
// @ Queue COM객체
// @ CommandList들을 GPU에 전달하는 목적으로 사용함.
// @ ExcuteCommandList() 함수로 명령을 전달
// @ Queue에 명령을 쌓았다가 적절한 시점에 제출
// @ 명령 예약 방식이라서 즉시 실행아님.

// @ 설명설명
class DescriptorHeap;

class CommandQueue
{
	// getter, setter
public:
	ComPtr<ID3D12CommandQueue>			GetCommandQueue		() { return _CommandQueue;			}
	ComPtr<ID3D12CommandAllocator>		GetCommandAllocator	() { return _CommandAllocator;		}
	ComPtr<ID3D12GraphicsCommandList>	GetCommandList		() { return _CommandList;			}
private:
	// @ 명령 큐, 명령 할당자, 명령 리스트 포인터
	ComPtr<ID3D12CommandQueue>			_CommandQueue;
	ComPtr<ID3D12CommandAllocator>		_CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList>	_CommandList;

	// 펜스 포인터, 펜스 값, 이벤트 핸들
	ID3D12Fence* _Fence;
	UINT64		_nFenceValue;
	HANDLE		_hFenceEvent;

// 초기화 함수
public:
	void Init(ComPtr<ID3D12Device> device);
};
