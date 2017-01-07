#include "player.h"
#include "common.h"
#include "math.h"
#include "graphics.h"
#include "game.h"

//TODO
#include <GLFW/glfw3.h>

vec2
Player::Pos()
{
	return _pos;
}

void
Player::Init(Game &game)
{
    Log("Loading player sprite");
	_spr = DEBUG_LoadSprite(game.GameDir + "/content/Player_Rifle.png");
	Log("Done");
}

void
Player::Update(Game &game)
{
	bool left = game.Input.Keyboard['A'];
	bool right = game.Input.Keyboard['D'];
	bool up = game.Input.Keyboard['W'];
	bool down = game.Input.Keyboard['S'];

	vec2 acc = {};
	if (left)
		acc.x -= 1;
	if (right)
		acc.x += 1;
	if (up)
		acc.y += 1;
	if (down)
		acc.y -= 1;

	if (game.Input.Keyboard[GLFW_KEY_ESCAPE])
		game.ShouldClose = true;

	acc = acc.Normalize() * 32.0f;
    if (sign(acc.x) != sign(_vel.x))
        acc.x += -_vel.x * 10;
    if (sign(acc.y) != sign(_vel.y))
        acc.y += -_vel.y * 10;


	_vel += acc * game.DT;

	float maxSpeed = 4.0f;
	if (game.Input.Keyboard[GLFW_KEY_LEFT_SHIFT])
        maxSpeed = 8.0f;

    if (_vel.Length() >= maxSpeed)
        _vel = _vel.Normalize() * maxSpeed;

    _pos += _vel * game.DT;

	auto mouse = ToGame(game, vec2{ game.Input.MouseX, game.Input.MouseY });

	_rot = std::atan2(mouse.y - _pos.y, mouse.x - _pos.x) - pi;
}

void
Player::Draw(Game &game)
{
	glm::mat3 modelView =
		Scale({ 2 / game.View.Width, 2 / game.View.Height }) *
		Translate({ -game.View.X, -game.View.Y }) *
		Translate({ _pos.x, _pos.y }) *
		Rotate(-_rot) *
		Scale({ 1, 1 });

	DEBUG_DrawSprite(_spr, modelView, FullImage, _rot);
}
