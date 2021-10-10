#pragma once
#include "pch.h"

class EngineCore
{
public:
	void Init(const WindowInfo& info);
private:
	shared_ptr<class Dx12Device>	_device;
	shared_ptr<class CommandQueue>	_cmdQueue;
	shared_ptr<class SwapChain>		_swapChain;
};
