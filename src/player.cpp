#include "player.h"
#include "common.h"
#include "entities/bullet.h"
#include "game.h"
#include "graphics.h"
#include "math.h"

// TODO
#include <GLFW/glfw3.h>

vec2
Player::Pos()
{
    return _pos;
}

Player::Player(Game& game, const vec2& pos)
  : GameComponent(game)
  , _pos(pos)
{
    Log("Loading player sprite");
    _spr =
      Engine.Content.LoadSprite(game.GameDir + "/content/Player_Rifle.png");
    Log("Done");
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

    auto potentialPos = _pos + _vel * Engine.DT;

    bool collision = false;

    Rectangle mask = { potentialPos.x, potentialPos.y, 0.5f, 0.5f };
    for (auto s : Engine.Statics) {
        collision = collision || s.Rect.Intersects(mask);
    }

    if (!collision) {
        _pos = potentialPos;
    }

    auto mouse =
      ToGame(Engine, vec2{ Engine.Input.MouseX, Engine.Input.MouseY });

    _rot = std::atan2(mouse.y - _pos.y, mouse.x - _pos.x) - pi;

    Engine.View.X = _pos.x;
    Engine.View.Y = _pos.y;

    if (Engine.Input.Mouse[0]) {
        Engine.AddComponent(new Bullet(Engine, _pos, _vel));
    }
}

void
Player::Draw()
{
    glm::mat3 modelView =
      Scale({ 2 / Engine.View.Width(), 2 / Engine.View.Height() }) *
      Translate({ -Engine.View.X, -Engine.View.Y }) *
      Translate({ _pos.x, _pos.y }) * Rotate(-_rot) * Scale({ 1, 1 });

    DEBUG_DrawSprite(_spr, modelView, FullImage, _rot);
}
