#include "playerHUD.h"

void
PlayerHUD::Draw()
{
    _font.RenderText(
        std::to_string(_player->_rifle->_ammo),
        Engine.Screen.Max() - vec2{90, 50},
        1,
        Colors::Black);


    _font.RenderText(
        std::to_string(_player->_rifle->_mags),
        Engine.Screen.Max() - vec2{ 50, 50 },
        1,
        Colors::Black);

    _player->_currentWeapon->DrawGUI();
}