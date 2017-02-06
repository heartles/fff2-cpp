#include "shotgun.h"

#include <random>

#include "../../math.h"
#include "../../player.h"
#include "../bullet.h"

const int MagSize = 12;
const float ReloadTime = 2;
const float Cooldown = 0.25f;

std::normal_distribution<float> randDist{ 0, pi / 16 };
std::random_device gen;

static float
Random()
{
    return Clamp<float>(randDist(gen), -pi / 10, pi / 10);
};

void
Shotgun::PrimaryTryFire()
{
    if (!_reloading && _cooldown == 0 && _ammo > 0) {
        for (int i = 0; i < 16; i++) {
            float r = Random();
            Engine.AddComponent(new Bullet(
              Engine, _player->Pos(),
              vec2::FromMagnitudeTheta(BulletSpeed, _player->Rot() + pi + r),
              _player->Rot() + r));
        }

        --_ammo;
        _cooldown = Cooldown;
    }
}

void
Shotgun::TryReload()
{
    if (_ammo < MagSize && _mags > 0 && !_reloading) {
        _reloadTimeLeft = ReloadTime;
        _reloading = true;
    }
}

void
Shotgun::DrawGUI()
{
}

void
Shotgun::SecondaryTryFire()
{
}

void
Shotgun::Update()
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