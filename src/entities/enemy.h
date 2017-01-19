#pragma once
#include "../game.h"
#include "../graphics.h"
#include "../player.h"

class Enemy : public GameComponent
{
    vec2 _pos;
    Sprite _spr;
    Player* _target;

  public:
    Enemy(vec2 pos, Game&);

    virtual void Update() override;
    virtual void Draw() override;
};
