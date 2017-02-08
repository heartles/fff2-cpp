#include "playerHUD.h"

#include "../entities/weapons/mg.h"
#include "../entities/weapons/rifle.h"
#include "../entities/weapons/shotgun.h"

void
PlayerHUD::Draw()
{
    _font.RenderText(std::to_string(_player->_rifle->_ammo),
                     Engine.Screen.Max() - vec2{ 90, 50 }, 1, Colors::Black);

    _font.RenderText(std::to_string(_player->_rifle->_mags),
                     Engine.Screen.Max() - vec2{ 50, 50 }, 1, Colors::Black);

    _font.RenderText(std::to_string(_player->_shotgun->_ammo),
                     Engine.Screen.Max() - vec2{ 90, 90 }, 1, Colors::Black);

    _font.RenderText(std::to_string(_player->_shotgun->_mags),
                     Engine.Screen.Max() - vec2{ 50, 90 }, 1, Colors::Black);

    if (_player->_mg->_ammo > 99) {
        _font.RenderText(std::to_string(_player->_mg->_ammo),
                         Engine.Screen.Max() - vec2{ 105, 130 }, 1,
                         Colors::Black);
    } else {
        _font.RenderText(std::to_string(_player->_mg->_ammo),
                         Engine.Screen.Max() - vec2{ 90, 130 }, 1,
                         Colors::Black);
    }

    _font.RenderText(std::to_string(_player->_mg->_mags),
                     Engine.Screen.Max() - vec2{ 50, 130 }, 1, Colors::Black);

    _player->_currentWeapon->DrawGUI();
}