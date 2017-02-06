#include "framerateStats.h"

#include "../game.h"

FramerateStats::FramerateStats(Game& g)
  : GUIComponent(g)
{
    auto s = Engine.Content.LoadShader(g.GameDir + "/content/text.gl.vert",
                                       g.GameDir + "/content/text.gl.frag");
    _font = DEBUG_LoadFont("C:/Windows/fonts/times.ttf", 32, s);
}

void
FramerateStats::Draw()
{
    frameCount++;
    frameCountTime += Engine.DT;

    if (frameCountTime > 1.0f) {
        fps = frameCount / frameCountTime;
        frameCount = 0;
        frameCountTime = 0.0f;
    }

    _font.RenderText("FPS: " + std::to_string(fps), { 0, 1080 - 24 }, 1,
                     Colors::Black);
}