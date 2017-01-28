#pragma once

#include "../gui.h"
#include "../player.h"

class PlayerHUD
    : public GUIComponent
{
    Player *_player;
    Font _font;

public:

    virtual void Draw() override;

    inline PlayerHUD(Player *p, Game &e) :_player(p), GUIComponent(e)
    {
        auto s = e.Content.LoadShader(e.GameDir + "/content/text.gl.vert", e.GameDir + "/content/text.gl.frag");
        _font = DEBUG_LoadFont("C:/Windows/fonts/times.ttf", 32, s);
    }
};