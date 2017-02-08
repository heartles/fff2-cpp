#include "player.h"

#include "common.h"
#include "entities/bullet.h"
#include "entities/weapons/mg.h"
#include "entities/weapons/rifle.h"
#include "entities/weapons/shotgun.h"
#include "game.h"
#include "graphics.h"
#include "gui/playerHUD.h"
#include "math.h"

// TODO
#include <GLFW/glfw3.h>

Player::Player(Game& game, const vec2& pos)
  : GameComponent(game)
  , _pos(pos)
{
    Log("Loading player sprite");
    _spr =
      Engine.Content.LoadSprite(game.GameDir + "/content/Player_Rifle.png");
    Log("Done");

    _rifle = new Rifle(this, Engine);
    Engine.AddComponent(_rifle);

    _shotgun = new Shotgun(this, Engine);
    Engine.AddComponent(_shotgun);

    _mg = new MG(this, Engine);
    Engine.AddComponent(_mg);

    _currentWeapon = _mg;

    Engine.AddComponent(new PlayerHUD(this, Engine));
}

void
Player::Update()
{
    bool left = Engine.Input.Keyboard['A'];
    bool right = Engine.Input.Keyboard['D'];
    bool up = Engine.Input.Keyboard['W'];
    bool down = Engine.Input.Keyboard['S'];

    vec2 acc = {};
    if (left)
        acc.x -= 1;
    if (right)
        acc.x += 1;
    if (up)
        acc.y += 1;
    if (down)
        acc.y -= 1;

    if (Engine.Input.Keyboard[GLFW_KEY_ESCAPE])
        Engine.ShouldClose = true;

    acc = acc.Normalize() * 24.0f;
    if (sign(acc.x) != sign(_vel.x))
        acc.x += -_vel.x * 10;
    if (sign(acc.y) != sign(_vel.y))
        acc.y += -_vel.y * 10;

    _vel += acc * Engine.DT;

    float maxSpeed = 4.0f;
    if (Engine.Input.Keyboard[GLFW_KEY_LEFT_SHIFT])
        maxSpeed = 8.0f;

    if (_vel.Length() >= maxSpeed)
        _vel = _vel.Normalize() * maxSpeed;

    vec2 potentialPos = _pos + _vel * Engine.DT;

    Rectangle mask = { potentialPos.x, potentialPos.y, 0.5f, 0.5f };
    ResolveCollision(mask, &potentialPos, Engine);

    _pos = potentialPos;

    auto mouse =
      ToGame(Engine, vec2{ Engine.Input.MouseX, Engine.Input.MouseY });

    _rot = std::atan2(mouse.y - _pos.y, mouse.x - _pos.x) - pi;

    Engine.View.X = _pos.x;
    Engine.View.Y = _pos.y;

    const float bulletSpeed = 22.0f;
    if (Engine.Input.Mouse[0]) {
        _currentWeapon->PrimaryTryFire();
    }

    if (Engine.Input.Keyboard['R'] && !Engine.OldInput.Keyboard['R']) {
        _currentWeapon->TryReload();
    }

    if (Engine.KeyPressed('1')) {
        _currentWeapon = _rifle;
    }

    if (Engine.KeyPressed('2')) {
        _currentWeapon = _shotgun;
    }

    if (Engine.KeyPressed('3')) {
        _currentWeapon = _mg;
    }
}

void
Player::Draw()
{
    Engine.View.DrawSpritePart(_spr, _pos, FullImage, { .5f, .5f }, _rot,
                               Colors::White);
}
