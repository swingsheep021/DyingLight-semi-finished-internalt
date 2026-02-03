#pragma once
#include <windows.h>
#include "imgui/imgui.h"
#include "Game.h"

class Menu
{
public:
    // 渲染菜单
    static void Render();

    // 显示/隐藏菜单
    static void ToggleMenu();

    // 获取菜单是否可见
    static bool IsMenuVisible();

private:
    static bool menuVisible;

    // 绘制测试界面
    static void DrawTestInterface();
};