#include "enemy.h"

Enemy::Enemy(vec2 pos, Game& g)
  : GameComponent(g)
  , _pos(pos)
{
    _target = g.GetInstanceOf<Player>();
    _spr = Engine.Content.LoadSprite(g.GameDir + "/content/burger_enemy.png");
}

void
Enemy::Update()
{
    auto target = _target->Pos();

    const float speed = 1.0f;

    auto vel = (target - _pos).Normalize() * speed * Engine.DT;

    auto pos = _pos + vel;

    ResolveCollision({ _pos.x, _pos.y, 0.5, 0.5 }, &pos, Engine);

    _pos = pos;
}

void
Enemy::Draw()
{
    Engine.View.DrawSpritePart(_spr, _pos, FullImage, { 0.5f, 0.5f }, 0, Colors::White);
}
