#pragma once
#include "../game.h"
#include "../graphics.h"

class Bullet :
    public GameComponent
{
    vec2 _pos, _vel;
    Sprite _spr;
public:
    Bullet(Game &g, const vec2 &pos, const vec2 &vel);
    virtual void Update() override;
    virtual void Draw() override;
};

