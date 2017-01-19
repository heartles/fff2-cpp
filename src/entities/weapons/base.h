#pragma once

#include "../../game.h"

class WeaponBase : public GameComponent
{
  public:
    virtual void PrimaryTryFire() = 0;
    virtual void SecondaryTryFire() = 0;
    virtual void TryReload() = 0;

  protected:
    inline WeaponBase(Game& g)
      : GameComponent(g)
    {
    }
};