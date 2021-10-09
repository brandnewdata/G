#pragma once
#include "pch.h"

class EngineCore
{
public:
	void Init();

private:
	shared_ptr<class Dx12Device>	_Device;
	shared_ptr<class CommandQueue>	_CommandQueue;
};
