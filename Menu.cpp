#include "Menu.h"
#include "Game.h"
#include "GameData.h"
#include"Debug.h"
#include <iostream>
bool Menu::menuVisible = true;

void Menu::Render()
{
    if (!menuVisible)
        return;

    DrawTestInterface();
}

void Menu::ToggleMenu()
{
    menuVisible = !menuVisible;
}

bool Menu::IsMenuVisible()
{
    return menuVisible;
}

void Menu::DrawTestInterface()
{
    



    float input_hp = 0.0f;
    ImGui::Begin("MyDyingLight_internal      by：Swing_Sheep", &menuVisible,
        ImGuiWindowFlags_AlwaysAutoResize);

    if (!pGameData || !pGameData->Module_Engine)
    {
        ImGui::Text("Error: pGameData is null or invalid");
        ImGui::End();
        return;
    }
    if (!pGame)
    {
        ImGui::Text("Error: pGame is null");
        ImGui::End();
        return;
    }
    if (!pGame->CheckAllPtr())
    {
        ImGui::Text("Error: Game data pointers are invalid");
        ImGui::End();
        return;
    }

    Engine::PlayerDI* LocalPlayer = pGame->pLocalClientDI ? pGame->pLocalClientDI->pPlayerDI : nullptr;
    if (!LocalPlayer)
    {
        ImGui::Text("Error: LocalPlayer is null");
        ImGui::End();
        return;
    }
    if (LocalPlayer->MaxHP <= 0)
    {
        ImGui::Text("Error: MaxHP is invalid");
        ImGui::End();
        return;
    }

    ImGui::Text("Current HP:%.0f/%.0f", LocalPlayer->HP, LocalPlayer->MaxHP);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(80.0f);
    ImGui::InputFloat("hpInput", &input_hp);
    ImGui::SameLine();

    if (ImGui::Button("change HP\n", ImVec2(80.0f, 0.0f)))
    {
        if (input_hp < 0) input_hp = 0;
        if (input_hp > LocalPlayer->MaxHP) input_hp = LocalPlayer->MaxHP;
        LocalPlayer->HP = input_hp;
    }

    ImGui::Text("Max HP：%.0f", LocalPlayer->MaxHP);
    ImGui::Text("X：%.1f", LocalPlayer->position.x);
    ImGui::Text("Y：%.1f", LocalPlayer->position.y);
    ImGui::Text("Z：%.1f", LocalPlayer->position.z);


    

    //printf("pGame:%X\n", pGame);
    //printf("pGameDI:%X\n",pGame->pGameDI);
    //printf("pSessionCooperativeDI:%X\n", pGame->pSessionCooperativeDI);
    //printf("pLocalClientDI:%X\n", pGame->pLocalClientDI);
    //printf("pLevelDI:%X\n", pGame->pLevelDI);
    //printf("pCDOEManger:%X\n", *(uintptr_t*)(pGame->pCodeManger));
    //printf("pCDOEManger:%X\n", pGame->pCodeManger);
    Engine::MarrList* MarryList =pGame->pCodeManger->pOne->pTwo->pThree->pMarrList;
    
    if (!MarryList)
    {
        MessageBoxA(0, "Marrylist wrong", 0, 0);
        return;
    }
    //printf("MarryList:%X\n", MarryList);
    //
    //printf("HumanAI  One:%X\n",MarryList->pCModelObject->pHumanAI);
    //printf("HumanAI Y:%f\n", MarryList->pCModelObject->pHumanAI->Postion.y);
    //printf("pCameraBase:%X\n", pGame->pSessionCooperativeDI->pCameraManager->pCamera->pCameraBase);
    //printf("Matrix:%f\n", (pGame->pSessionCooperativeDI->pCameraManager->pCamera->pCameraBase->Matrix[0]));

 
    // 2. 敌人列表安全读取
// 1. 获取背景画板
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // 2. 严谨的指针链条校验
    if (pGame && pGame->pCodeManger) {
        Engine::CodeManger* codeMgr = pGame->pCodeManger;

        // 逐级检查指针是否有效
        if (codeMgr->pOne && codeMgr->pOne->pTwo && codeMgr->pOne->pTwo->pThree) {
            Engine::MarrList* pList = codeMgr->pOne->pTwo->pThree->pMarrList;

            if (pList) {
                for (int i = 0; i < 5; i++) {
                    // 根据 Engine.h 定义，pList[i] 是一个 MarrList 对象（包含 pCModelObject 指针）
                    Engine::CModelObject* pModel = pList[i].pCModelObject;

                    // 基础合法性检查
                    if (!pModel || (uintptr_t)pModel < 0x1000000) continue;

                    Engine::HumanAI* pAI = pModel->pHumanAI;
                    if (!pAI || (uintptr_t)pAI < 0x1000000) continue;

                    // 3. 获取敌人世界坐标
                    Vector3 worldPos = pAI->Postion;

                    // 过滤无效坐标
                    if (worldPos.x == 0 && worldPos.y == 0) continue;

                    Vector2 screenPos;
                    // 4. 调用 W2S 进行转换并绘制
                    if (pGame->GameraToScreen(&worldPos, &screenPos)) {
                        // 画一个黄色的圈表示敌人
                        drawList->AddCircle(
                            ImVec2(screenPos.x, screenPos.y),
                            10.0f,
                            IM_COL32(255, 255, 0, 255),
                            12,     // 分段数
                            2.0f    // 线宽
                        );
                    }
                }
            }
        }
    }

    ImGui::End(); // 务必保证 End 在最后，不被任何 if 截断
}