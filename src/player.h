#pragma once

#include <glm/glm.hpp>

#include "entities/weapons/rifle.h"
#include "game.h"
#include "graphics.h"
#include "math.h"

class Player : public GameComponent
{
    Sprite _spr;
    vec2 _pos;
    vec2 _vel;
    float _rot;

    Rifle* _rifle;
    WeaponBase* _currentWeapon;

  public:
    inline const vec2& Pos() const { return _pos; }

    inline float Rot() const { return _rot; }

    Player(Game& game, const vec2& pos);

    virtual void Update() override;
    virtual void Draw() override;
};
