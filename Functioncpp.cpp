#include "Function.h"



float Function::GetHealth(Engine::PlayerDI* player)
{
    if (!player)
        return 0.0f;

    return player->HP;
}

float Function::GetMaxHealth(Engine::PlayerDI* player)
{
    if (!player)
        return 0.0f;

    return player->MaxHP;
}

void Function::SetHealth(Engine::PlayerDI* player, float value)
{
    if (!player)
        return;

    player->HP = value;
}

void Function::GetPosition(Engine::PlayerDI* player,pVector3 pVector3)
{
    if (!player)
    {
        return;
    }

    pVector3->x = player->position.x;
    pVector3->y= player->position.y;
    pVector3->z = player->position.z;
}

void Function::SetPosition(Engine::PlayerDI* player, pVector3 pVector3)
{
    if (!player)
        return;
    player->position.x = pVector3->x;
    player->position.y = pVector3->y;
    player->position.z = pVector3->z;
}