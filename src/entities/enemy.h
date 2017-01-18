#pragma once
#include "../game.h"
#include "../player.h"
#include "../graphics.h"

class Enemy :
    public GameComponent
{
public:

    Sprite _spr;
    Player *_target;

    Enemy(Game &);

    virtual void Update() override;
    virtual void Draw() override;
};

