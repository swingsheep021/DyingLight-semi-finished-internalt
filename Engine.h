#pragma once
#include <cstdint>
#include "GameData.h"
#include "Offest.h"
#include "SafeRead.h"
#include "imgui/imgui.h"
typedef struct Vector3
{
	float x;
	float y;
	float z;
}Vector3, * pVector3;

typedef struct Vector2
{
	float x;
	float y;
}Vector2, * pVector2;



namespace Engine{

	class CGame
	{
	public:
		char pad_0000[32]; //0x0000
		class GameDI* pGameDI; //0x0020
		static CGame* GetInstance()
		{
			static CGame* pCGame = nullptr;
			if (pCGame == nullptr)
			{
				if (!pGameData || !pGameData->Module_Engine)
					return nullptr;
				CGame* p = static_cast<CGame*>(SafeReadPointerAt(pGameData->Module_Engine, Offset::CGame));
				if (!p || reinterpret_cast<uintptr_t>(p) < 0x10000)
					return nullptr;
				pCGame = p;
			}
			return pCGame;
		}
	};

	class GameDI
	{
	public:
		char pad_0000[1264]; //0x0000
		class SessionCooperativeDI* pSessionCooperativeDI; //0x04F0
	};

	class SessionCooperativeDI
	{
	public:
		char pad_0000[176]; //0x0000
		class LevelDI* pLevelDI; //0x00B0
		class LocalClientDI* pLocalClientDI; //0x00B8
		class CameraManager* pCameraManager; //0x00C0
	};

	class LocalClientDI
	{
	public:
		char pad_0000[88]; //0x0000
		class PlayerDI* pPlayerDI; //0x0058
	};

	class PlayerDI {
	public:
		char pad_0000[2024]; //0x0000
		Vector3 position;    //0x7E8
		char pad_07F4[2616]; //0x07F4
		float HP; //0x122C
		float MaxHP; //0x1230
	};

	class LevelDI
	{
	public:
		char pad_0000[8]; //0x0000
		class Level* pLevel; //0x0008
		//char pad_0010[3856]; //0x0010
		//DWORD64* MarryList; //0x0F20
	}; //Size: 0x0F28

	//class Level
	//{
	//public:
	//	char pad_0000[2344]; //0x0000
	//	char ModelList[1][8]; //0x0928
	//};

	class CameraManager
	{
	public:
		char pad_0000[80]; //0x0000
		class Camera* pCamera; //0x0050
	};

	class Camera
	{
	public:
		char pad_0000[8]; //0x0000
		class CameraBase* pCameraBase; //0x0008
	};

	class CameraBase
	{
	public:
		char pad_0000[176]; //0x0000
		float Matrix[16]; //0x0010
	};

	//class HumnAI
	//{
	//public:
	//	char pad_0000[1216]; //0x0000
	//	Vector3 Postion; //0x04C0
	//	char pad_04CC[280]; //0x04CC
	//	int32_t zombie_state; //0x05E4
	//	char pad_05E8[1024]; //0x05E8
	//	class ArmorHealthModule* pArmorHealthModule; //0x09E8
	//};


	class ArmorHealthModule
	{
	public:
		char pad_0000[116]; //0x0000
		int32_t zombie_type; //0x0074
		float HP; //0x0078
		char pad_007C[76]; //0x007C
		float HP_MAX; //0x00C8
	};




	class CodeManger
	{
	public:
		char pad_0000[2232]; //0x0000
		class One* pOne; //0x08B8
	}; //Size: 0x08C0


	class One
	{
	public:
		char pad_0000[2416];
		class Two* pTwo; //0x0000
	}; //Size: 0x0008


	class Two
	{
	public:
		char pad_0000[48]; //0x0000
		class Three* pThree; //0x0030
	}; //Size: 0x0038


	class Three
	{
	public:
		char pad_0000[448]; //0x0000
		class MarrList* pMarrList; //0x01C0
	}; //Size: 0x01C8

	class MarrList
	{
	public:
		class CModelObject* pCModelObject; //0x0000
	}; //Size: 0x0008

	class CModelObject
	{
	public:
		char pad_0000[160]; //0x0000
		class HumanAI* pHumanAI; //0x00A0
	}; //Size: 0x00A8


	class HumanAI
	{
	public:
		char pad_0000[2004]; //0x0000
		Vector3 Postion; //0x07D4
	}; //Size: 0x07E0


}

