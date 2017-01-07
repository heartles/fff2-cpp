#pragma once

#include <glm/glm.hpp>
#include "graphics.h"
#include "math.h"

struct Game;

struct Player
{
    Sprite _spr;
    vec2 _pos;
    vec2 _vel;
    float _rot;

	vec2 Pos();
	void Init(Game&);
	void Update(Game&);
	void Draw(Game&);
};
