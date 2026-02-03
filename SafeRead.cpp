#include "SafeRead.h"

// 仅使用基本类型，无 C++ 对象，允许使用 __try/__except
void* SafeReadPointerAt(DWORD64 base, DWORD64 offset)
{
    void* result = nullptr;
    __try
    {
        result = *reinterpret_cast<void**>(base + offset);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        return nullptr;
    }
    return result;
}
