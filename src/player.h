#pragma once

#include <glm/glm.hpp>

#include "entities/weapons/base.h"
#include "game.h"
#include "graphics.h"
#include "math.h"

class Rifle;
class Shotgun;
class MG;
class Player : public GameComponent
{
    Sprite _spr;
    vec2 _pos;
    vec2 _vel;
    float _rot;

    Rifle* _rifle;
    Shotgun* _shotgun;
    MG* _mg;
    WeaponBase* _currentWeapon;

    friend class PlayerHUD;

  public:
    inline const vec2& Pos() const { return _pos; }

    inline float Rot() const { return _rot; }

    Player(Game& game, const vec2& pos);

    virtual void Update() override;
    virtual void Draw() override;
};
