#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

struct Sprite
{
	int Width;
	int Height;
	std::vector<uint8_t> Memory;
    GLuint TextureID;
};

struct Rectangle
{
	float X, Y, Width, Height;
};

const struct Rectangle FullImage{ 0, 0, 1, 1 };

struct View
{
    struct Rectangle Game, Display;
};

Sprite
DEBUG_LoadSprite(std::string filename);

//void
//SetUniform(std::string name, const b2Color &color);

void
SetUniform(std::string name, const glm::mat3 &value);

void
DEBUG_DrawSprite(Sprite spr, glm::mat3, struct Rectangle, float);
