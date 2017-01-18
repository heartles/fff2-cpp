#include "enemy.h"

Enemy::Enemy(Game &g)
    : GameComponent(g)
{
    _target = g.GetInstanceOf<Player>();
    _spr = DEBUG_LoadSprite(g.GameDir + "/content/Burger_spr_0.png");
}

void
Enemy::Update()
{

}

void
Enemy::Draw()
{
    DEBUG_DrawSprite();
}

