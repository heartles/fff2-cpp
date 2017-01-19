#pragma once

#include "game.h"
#include "graphics.h"
#include "math.h"
#include <glm/glm.hpp>

class Player : public GameComponent
{
    Sprite _spr;
    vec2 _pos;
    vec2 _vel;
    float _rot;


  public:
    const vec2 & Pos();
    Player(Game& game, const vec2& pos);

    virtual void Update() override;
    virtual void Draw() override;
};
