#pragma once
// C++ STD 라이브러리와 윈도우즈 헤더
#include <windows.h> // C++17 이상 사용하면 using namespace std 보다 이 헤더를 먼저 추가 해야 오류가 발생하지 않는다.
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
using namespace std; // C++17 이상 사용하면 windoes.h를 먼저 추가 해야 오류가 발생하지 않는다.

// DirextX 관련 헤더들
#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

// 정적 라이브러리들
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// Typedefs
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= XMFLOAT2;
using Vec3		= XMFLOAT2;
using Vec4		= XMFLOAT2;
using Matrix	= XMMATRIX;

namespace ECoreSetting
{
	enum
	{
		SWAP_CHAIN_BUFFER_COUNT = 2 // 더블 버퍼링을 할 것 이다!
	};
}



struct WindowInfo
{
	HWND	hWnd;		// 출력 윈도우 핸들
	int32	width;		// 출력 화면 너비
	int32	height;		// 출력 화면 높이
	bool	bWindowed;	// 창모드인가?
};

