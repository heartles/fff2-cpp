#pragma once
#include "../game.h"
#include "../player.h"
#include "../graphics.h"

class Enemy :
    public GameComponent
{
    vec2 _pos;
    Sprite _spr;
    Player *_target;
public:

    Enemy(vec2 pos, Game &);

    virtual void Update() override;
    virtual void Draw() override;
};

