#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "math.h"

struct Sprite
{
    int Width;
    int Height;
    std::vector<uint8_t> Memory;
    GLuint TextureID;
};

struct Rectangle
{
    float X, Y, HalfWidth, HalfHeight;

    inline vec2 Min() const { return { X - HalfWidth, Y - HalfHeight }; }

    inline vec2 Max() const { return { X + HalfWidth, Y + HalfHeight }; }

    inline vec2 Pos() const { return { X, Y }; }

    inline float Width() const { return HalfWidth * 2; }
    inline float Height() const { return HalfHeight * 2; }

    inline vec2 Dim() const { return { HalfWidth * 2, HalfHeight * 2 }; }

    inline bool Intersects(const Rectangle& other) const
    {
        return Min().x < other.Max().x && Max().x > other.Min().x &&
               Min().y < other.Max().y && Max().y > other.Min().y;
    }

    static inline Rectangle FromCorner(vec2 mincorner, float w, float h)
    {
        Rectangle res{};
        res.HalfWidth = w / 2;
        res.HalfHeight = h / 2;
        res.X = mincorner.x + res.HalfWidth;
        res.Y = mincorner.y + res.HalfHeight;

        return res;
    }
};

const struct Rectangle FullImage = Rectangle::FromCorner({ 0.0f, 0.0f }, 1, 1);

struct View
{
    struct Rectangle Game, Display;
};

Sprite DEBUG_LoadSprite(std::string filename);

// void
// SetUniform(std::string name, const b2Color &color);

void SetUniform(std::string name, const glm::mat3& value);

void DEBUG_DrawSprite(Sprite spr, glm::mat3, struct Rectangle sprPart,
                      float rot);
