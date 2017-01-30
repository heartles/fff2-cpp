#pragma once

#include "base.h"

class Player;
class Rifle : public WeaponBase
{
    friend class PlayerHUD;

    Player* _player;

    int _ammo = 12, _mags = 12;

    // zero when not reloading
    float _reloadTimeLeft = 0;
    bool _reloading = false;

    // zero when cooldown over
    float _cooldown = 0;

  public:
    virtual void PrimaryTryFire() override;
    virtual void SecondaryTryFire() override;
    virtual void TryReload() override;
    virtual void Update() override;
    virtual void DrawGUI() override;

    Rifle(Player*, Game&);
};
