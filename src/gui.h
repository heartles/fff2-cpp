#pragma once

#include "game.h"
class Game;
class GUIComponent
{
    virtual void Draw() = 0;
    virtual ~GUIComponent() = 0;

    Game &Engine;
protected:
    inline GUIComponent(Game &g)
        : Engine(g)
    {}
};
