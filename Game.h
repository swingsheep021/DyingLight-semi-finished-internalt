#pragma once
#include"Engine.h"

class Game {
public:
	void Update();

	bool CheckAllPtr();

	bool GameraToScreen(IN pVector3 ZombiePOS, OUT Vector2* Ret);
	Engine::CGame* pCGame = nullptr;
	Engine::GameDI* pGameDI = nullptr;
	Engine::SessionCooperativeDI* pSessionCooperativeDI = nullptr;
	Engine::LocalClientDI* pLocalClientDI = nullptr;
	Engine::LevelDI* pLevelDI = nullptr;
	Engine::CameraManager* pCameraManager = nullptr;
	Engine::CodeManger* pCodeManger = nullptr;
};

inline Game* pGame = nullptr;
