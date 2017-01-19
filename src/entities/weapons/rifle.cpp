#include "rifle.h"

#include "../../player.h"
#include "../bullet.h"

const float BulletSpeed = 22.0f;
const int MagSize = 12;
const float Cooldown = 0.25f;
const float ReloadTime = 2;

void
Rifle::PrimaryTryFire()
{
    if (_ammo > 0 && _cooldown == 0 && _reloadTimeLeft == 0) {
        Engine.AddComponent(
          new Bullet(Engine, _player->Pos(),
                     vec2::FromMagnitudeTheta(BulletSpeed, _player->Rot() + pi),
                     _player->Rot()));

        --_ammo;
        _cooldown = Cooldown;
    }
}

void
Rifle::SecondaryTryFire()
{
}

void
Rifle::TryReload()
{
    if (_ammo < MagSize && _mags > 0) {
        _ammo = MagSize;
        --_mags;
        _reloadTimeLeft = ReloadTime;
    }
}

void
Rifle::Update()
{
    _cooldown -= Engine.DT;

    if (_cooldown < 0) {
        _cooldown = 0;
    }

    _reloadTimeLeft -= Engine.DT;

    if (_reloadTimeLeft < 0) {
        _reloadTimeLeft = 0;
    }
}

Rifle::Rifle(Player* p, Game& g)
  : _player(p)
  , WeaponBase(g)
{
}
