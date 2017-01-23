#pragma once

#include "../gui.h"
#include "../player.h"

class PlayerHUD
    : public GUIComponent
{
    Player *_player;
};