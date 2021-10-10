#pragma once
#include "pch.h"

class EngineCore
{
public:
	void Init(const WindowInfo& info);
private:
	shared_ptr<class Dx12Device>	_Device;
	shared_ptr<class CommandQueue>	_CommandQueue;
	shared_ptr<class SwapChain>		_SwapChain;
};
