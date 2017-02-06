#pragma once

#include "../gui.h"

#include "../graphics.h"

class FramerateStats : public GUIComponent
{
    int frameCount = 0;
    float frameCountTime = 0, fps = 0;
    Font _font;

  public:
    FramerateStats(Game& g);
    virtual void Draw() override;
};
