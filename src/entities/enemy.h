#pragma once
#include "../game.h"
#include "../graphics.h"
#include "../player.h"

class Enemy : public GameComponent
{
    vec2 _pos;
    Sprite _spr;
    Player* _target;

    float _health = 75;

  public:
    Enemy(vec2 pos, Game&);

    virtual void Update() override;
    virtual void Draw() override;
    virtual void TakeDamage(float dmg);

    inline Rectangle Mask() { return { _pos.x, _pos.y, 0.5, 0.5 }; }
};
