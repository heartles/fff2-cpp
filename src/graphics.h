#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "math.h"
#include "shader.h"

struct Sprite
{
    int Width;
    int Height;
    std::vector<uint8_t> Memory;
    GLuint TextureID;
};

namespace Colors
{
    const vec4 Black = { 0,0,0,1 };
    const vec4 White = { 1,1,1,1 };
    const vec4 Red = { 1,0,0,1 };
    const vec4 Green = { 0,1,0,1 };
    const vec4 Blue = { 0,0,1,1 };
};

struct Character
{
    Sprite Sprite;
    ivec2 Offset; // Offset from origin to left/top of char
    ivec2 Advance; // Offset to next char/line
};

class Font
{
    std::map<uint64_t, Character> _codepoints;
    Shader _shader;

public:
    inline Font() {}

    inline Font(std::map<uint64_t, Character> &&p, Shader s) : _codepoints(p), _shader(s)
    {}

    void RenderText(std::string, ivec2 pos, float scale, vec4 color);
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

Font DEBUG_LoadFont(std::string filename, int pxSize, Shader s);