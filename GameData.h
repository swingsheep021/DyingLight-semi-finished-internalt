#pragma once
#include <windows.h>

class GameData
{
public:

	GameData();

	// 初始化获取模块偏移

	DWORD64 Module_Engine = 0;
	DWORD64 Module_GameDLL = 0;

};

extern GameData* pGameData;

//inline GameData::GameData()
//{
//	this->Module_Engine = (DWORD64)GetModuleHandleA("engine_x64_rwdi.dll");
//	this->Module_GameDLL = (DWORD64)GetModuleHandleA("gamedll_x64_rwdi.dll");
//}

//inline bool GameData::InitOffsets()
//{
//	// 简单的初始化，实际项目中可能需要更复杂的偏移量搜索
//	return true;
//}