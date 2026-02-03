#pragma once
#include <Windows.h>

// 从 base+offset 安全读取指针（非法地址时返回 nullptr，不崩溃）
// 仅在无 C++ 对象展开的 .cpp 中使用 __try，避免编译器错误
void* SafeReadPointerAt(DWORD64 base, DWORD64 offset);
