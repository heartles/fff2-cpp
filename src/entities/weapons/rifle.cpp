#include "rifle.h"

#include "../../player.h"
#include "../bullet.h"

const int MagSize = 12;
const float Cooldown = 0.25f;
const float ReloadTime = 2;

void
Rifle::PrimaryTryFire()
{
    if (_ammo > 0 && _cooldown == 0 && !_reloading) {
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
    if (_ammo < MagSize && _mags > 0 && !_reloading) {
        _reloadTimeLeft = ReloadTime;
        _reloading = true;
    }
}

void
Rifle::Update()
{
    _cooldown -= Engine.DT;

    if (_cooldown < 0) {
        _cooldown = 0;
    }

    if (_reloading) {
        _reloadTimeLeft -= Engine.DT;

        if (_reloadTimeLeft < 0) {
            _ammo = MagSize;
            --_mags;
            _reloadTimeLeft = 0;
            _reloading = false;
        }
    }
}

void
Rifle::DrawGUI()
{
    // TODO: reloading indicator code
}

Rifle::Rifle(Player* p, Game& g)
  : _player(p)
  , WeaponBase(g)
{
}
