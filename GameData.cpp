#include "GameData.h"

GameData* pGameData = nullptr;

GameData::GameData() {
    Module_Engine = (DWORD64)GetModuleHandleA("engine_x64_rwdi.dll");
    Module_GameDLL = (DWORD64)GetModuleHandleA("gamedll_x64_rwdi.dll");
}