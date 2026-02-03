#include "Game.h"
#include "GameData.h"

// 更新游戏数据（仅在渲染线程调用，避免多线程读游戏内存导致崩溃）
void Game::Update()
{
	if (!pGameData || !pGameData->Module_Engine)
		return;

	this->pCGame = Engine::CGame::GetInstance();

	this->pCodeManger = *(Engine::CodeManger**)(pGameData->Module_Engine + 0xA2C148);
	if (!pCodeManger)
	{
		MessageBoxA(0, "pCodeManger wrong", 0, 0);
		return;
	}
	


	if (!this->pCGame)
		return;

	this->pGameDI = this->pCGame->pGameDI;
	if (!this->pGameDI)
		return;

	this->pSessionCooperativeDI = this->pGameDI->pSessionCooperativeDI;
	if (!this->pSessionCooperativeDI)
		return;

	this->pLevelDI = this->pSessionCooperativeDI->pLevelDI;
	this->pLocalClientDI = this->pSessionCooperativeDI->pLocalClientDI;
	this->pCameraManager = this->pSessionCooperativeDI->pCameraManager;
	if (!this->pLocalClientDI || !this->pLocalClientDI->pPlayerDI)
		return;
}

// 检查所有指针是否有效
bool Game::CheckAllPtr() {
	if (this->pCGame == nullptr)
		return false;
	if (this->pGameDI == nullptr)
		return false;
	if (this->pSessionCooperativeDI == nullptr)
		return false;
	if (this->pLocalClientDI == nullptr)
		return false;
	if (this->pLocalClientDI->pPlayerDI == nullptr)
		return false;
	if (this->pLevelDI == nullptr)
		return false;
	if (this->pCameraManager == nullptr)
		return false;

	return true;
}


//// 1. 记得改成引用 &Ret
//bool Game::GameraToScreen(pVector3 ZombiePOS, Vector2* Ret) {
//	if (!Ret || !ZombiePOS) return false;
//
//	// 严谨的链条检查
//	if (!this->pCameraManager || !this->pCameraManager->pCamera || !this->pCameraManager->pCamera->pCameraBase)
//		return false;
//
//	float* Matrix = this->pCameraManager->pCamera->pCameraBase->Matrix;
//	if (!Matrix) return false;
//
//	float w = Matrix[12] * ZombiePOS->x + Matrix[13] * ZombiePOS->y + Matrix[14] * ZombiePOS->z + Matrix[15];
//
//	if (w < 0.01f) return false;
//
//	float x = Matrix[0] * ZombiePOS->x + Matrix[1] * ZombiePOS->y + Matrix[2] * ZombiePOS->z + Matrix[3];
//	float y = Matrix[4] * ZombiePOS->x + Matrix[5] * ZombiePOS->y + Matrix[6] * ZombiePOS->z + Matrix[7];
//
//	float inw = 1.0f / w;
//	ImVec2 display_size = ImGui::GetIO().DisplaySize;
//
//	Ret->x = (display_size.x / 2.0f) + (x * inw * (display_size.x / 2.0f));
//	Ret->y = (display_size.y / 2.0f) - (y * inw * (display_size.y / 2.0f));
//
//	return true;
//}
//
bool Game::GameraToScreen(pVector3 ZombiePOS, Vector2* Ret) {
	if (!Ret || !ZombiePOS) return false;

	if (!this->pCameraManager || !this->pCameraManager->pCamera || !this->pCameraManager->pCamera->pCameraBase)
		return false;

	float* Matrix = this->pCameraManager->pCamera->pCameraBase->Matrix;

	// --- 尝试“列主序”计算 (Transposed Matrix) ---
	// X 分量使用：0, 4, 8, 12
	float x = Matrix[0] * ZombiePOS->x + Matrix[4] * ZombiePOS->y + Matrix[8] * ZombiePOS->z + Matrix[12];
	// Y 分量使用：1, 5, 9, 13
	float y = Matrix[1] * ZombiePOS->x + Matrix[5] * ZombiePOS->y + Matrix[9] * ZombiePOS->z + Matrix[13];
	// W 分量使用：3, 7, 11, 15
	float w = Matrix[3] * ZombiePOS->x + Matrix[7] * ZombiePOS->y + Matrix[11] * ZombiePOS->z + Matrix[15];

	if (w < 0.01f) return false;

	float invW = 1.0f / w;
	ImVec2 display_size = ImGui::GetIO().DisplaySize;

	// 标准化设备坐标 -> 屏幕像素坐标
	Ret->x = (display_size.x / 2.0f) * (1.0f + x * invW);
	Ret->y = (display_size.y / 2.0f) * (1.0f - y * invW);

	return true;
}