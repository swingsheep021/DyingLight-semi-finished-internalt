#pragma once
#include <Windows.h>

namespace Offset {
	// CGame 全局指针地址 = engine_x64_rwdi.dll + A29C08
	inline DWORD64 CGame = 0xA29C08;
	inline DWORD64 CODE = 0xA2C148;
}