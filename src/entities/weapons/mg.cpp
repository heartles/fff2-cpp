#include "mg.h"

#include <random>

#include "../../math.h"
#include "../../player.h"
#include "../bullet.h"

const int MagSize = 100;
const float ReloadTime = 3;
const float Cooldown = 0.1f;

static std::normal_distribution<float> randDist{ 0, pi / 32 };
static std::random_device gen;

static float
Random()
{
    return Clamp<float>(randDist(gen), -pi / 16, pi / 16);
};

void
MG::PrimaryTryFire()
{
    if (!_reloading && _cooldown == 0 && _ammo > 0) {
        float r = Random() * _recoil;
        Engine.AddComponent(new Bullet(
          Engine, _player->Pos(),
          vec2::FromMagnitudeTheta(BulletSpeed, _player->Rot() + pi + r),
          _player->Rot() + r));

        _recoil = Min(1.5f, _recoil + 0.1f);

        --_ammo;
        _cooldown = Cooldown;
    }
}

void
MG::TryReload()
{
    if (_ammo < MagSize && _mags > 0 && !_reloading) {
        _reloadTimeLeft = ReloadTime;
        _reloading = true;
    }
}

void
MG::DrawGUI()
{
}

void
MG::SecondaryTryFire()
{
}

void
MG::Update()
{
    _cooldown -= Engine.DT;

    _recoil = Max(0.0f, _recoil - Engine.DT / 2);

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