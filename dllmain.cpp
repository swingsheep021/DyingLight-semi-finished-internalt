#include <Windows.h>
#include <dxgi.h>
#include "DirectXHook/DirectXHook.h"
#include "ImGuiHook/ImGuiHook.h"
#include "Menu.h"
#include "Game.h"
#include "GameData.h"
#include <iostream>


// 全局变量，用于控制主线程退出
bool g_bRunning = true;

// 原始D3D9 Present函数
typedef HRESULT(__stdcall* D3D9Present_t)(LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);
D3D9Present_t oD3D9Present = nullptr;

// 游戏数据更新节流：每 UPDATE_INTERVAL_MS 毫秒才更新一次，减轻卡顿
#define UPDATE_INTERVAL_MS 100

// D3D9 Present函数钩子
HRESULT __stdcall hkD3D9Present(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
    static bool init = false;
    static ImGuiHook imguiHook;
    static ULONGLONG lastUpdateTick = 0;

    if (!init)
    {
        if (!pDevice)
            return oD3D9Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
        if (!imguiHook.Init(DXTYPE::D3D9, pDevice))
            return oD3D9Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
        init = true;
    }
    ULONGLONG now = GetTickCount64();
    if (pGame && (now - lastUpdateTick >= UPDATE_INTERVAL_MS))
    {
        lastUpdateTick = now;
        pGame->Update();
    }
    imguiHook.StartFrame();
    if (GetAsyncKeyState(VK_INSERT) & 1)
        Menu::ToggleMenu();
    Menu::Render();
    imguiHook.EndFrame();
    return oD3D9Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

// 原始D3D11 Present函数
typedef HRESULT(__stdcall* D3D11Present_t)(IDXGISwapChain*, UINT, UINT);
D3D11Present_t oD3D11Present = nullptr;

// D3D11 Present函数钩子
HRESULT __stdcall hkD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    static bool init = false;
    static ImGuiHook imguiHook;
    static ID3D11Device* device = nullptr;
    static ID3D11DeviceContext* deviceContext = nullptr;
    static ULONGLONG lastUpdateTick = 0;

    if (!init)
    {
        HRESULT hr = pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
        if (FAILED(hr) || !device)
            return oD3D11Present(pSwapChain, SyncInterval, Flags);
        device->GetImmediateContext(&deviceContext);
        if (!deviceContext)
            return oD3D11Present(pSwapChain, SyncInterval, Flags);
        HWND gameWindow = nullptr;
        DXGI_SWAP_CHAIN_DESC scDesc = {};
        if (SUCCEEDED(pSwapChain->GetDesc(&scDesc)))
            gameWindow = scDesc.OutputWindow;
        if (!imguiHook.Init(DXTYPE::D3D11, device, gameWindow))
            return oD3D11Present(pSwapChain, SyncInterval, Flags);
        init = true;
    }
    ULONGLONG now = GetTickCount64();
    if (pGame && (now - lastUpdateTick >= UPDATE_INTERVAL_MS))
    {
        lastUpdateTick = now;
        pGame->Update();
    }
    imguiHook.StartFrame();
    if (GetAsyncKeyState(VK_INSERT) & 1)
        Menu::ToggleMenu();
    Menu::Render();
    imguiHook.EndFrame();
    return oD3D11Present(pSwapChain, SyncInterval, Flags);
}



// 使用安全的 freopen_s 替代 freopen
void OpenDebugConsole() {
    AllocConsole();

    FILE* fp;
    // 使用 freopen_s 重定向标准输出
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONIN$", "r", stdin);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    std::cout << "=== 调试控制台已开启 ===" << std::endl;
}



// 主线程函数（只负责等待 + 初始化钩子，不再读游戏内存）
DWORD WINAPI MainThread(LPVOID lpParam)
{

    OpenDebugConsole();

    for (int i = 0; i < 80; i++)
    {
        if (GetModuleHandleA("d3d11.dll") != NULL && GetModuleHandleA("engine_x64_rwdi.dll") != NULL)
            break;
        Sleep(100);
    }
    Sleep(1500);

    if (!pGameData)
        pGameData = new GameData();
    if (!pGame)
        pGame = new Game();

    if (!DirectXHook::get().Init(DXTYPE::D3D11, (void**)&oD3D11Present, hkD3D11Present))
    {
        if (!DirectXHook::get().Init(DXTYPE::D3D9, (void**)&oD3D9Present, hkD3D9Present))
        {
            if (pGame) { delete pGame; pGame = nullptr; }
            if (pGameData) { delete pGameData; pGameData = nullptr; }
            return 1;
        }
    }

    while (g_bRunning)
        Sleep(100);
    return 0;
}

// DllMain函数
BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:        // 当DLL被加载时执行
    {
        // 禁用线程库调用，防止线程库调用导致的错误行为
        DisableThreadLibraryCalls(hModule);

        // 创建主线程
        HANDLE hThread = CreateThread(NULL, 0, MainThread, hModule, 0, NULL);
        if (hThread != NULL)
        {
            CloseHandle(hThread);
        }
        break;
    }

    case DLL_PROCESS_DETACH:        // 当DLL被卸载时执行
    {
        // 设置线程退出标志
        g_bRunning = false;

        // 给线程一点时间退出
        Sleep(20);

        // 卸载DirectX钩子
        bool hookEndSuccess = DirectXHook::get().End();
        (void)hookEndSuccess; // 即使钩子卸载失败，也继续清理其他资源

        // 清理pGame指针
        if (pGame)
        {
            delete pGame;
            pGame = nullptr;
        }

        // 清理pGameData指针
        if (pGameData)
        {
            delete pGameData;
            pGameData = nullptr;
        }
        break;
    }
    }


    return TRUE;
}