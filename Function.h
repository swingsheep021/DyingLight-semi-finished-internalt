#pragma once
#include "Game.h"
#include "Memory/Memory.h"
#include "Engine.h"



namespace Function
{

	//��ȡ�������ֵ
	float GetHealth(Engine::PlayerDI* player);

	//��ȡ����������ֵ
	float GetMaxHealth(Engine::PlayerDI* player);

	//�����������ֵ
	void SetHealth(Engine::PlayerDI* player, float value);

	//��ȡ�������
	void GetPosition(Engine::PlayerDI* player, pVector3 pVector3);

	//�����������
	void SetPosition(Engine::PlayerDI* player, pVector3 pVector3);



}