#include "graphics.h"

#include <exception>

#include "common.h"
#include "content.h"
#include "game.h"
#include "glad/glad.h"
#include "stb/image.h"

using namespace std;

Sprite
DEBUG_LoadSprite(std::string filename)
{
    Log("loading sprite " + filename);
    Sprite s;
    uint8_t* mem = stbi_load(filename.c_str(), &s.Width, &s.Height, nullptr, 4);
    if (!mem)
        throw std::exception("file load failed");
    s.Memory.assign(mem, mem + s.Width * s.Height * 4);

    glGenTextures(1, &s.TextureID);
    glBindTexture(GL_TEXTURE_2D, s.TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s.Width, s.Height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, s.Memory.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    return s;
}

GLuint vertexArrayID;
GLuint vertexBuffers[2];
GLuint& locationBuffer = vertexBuffers[0];
GLuint& uvBuffer = vertexBuffers[1];

vector<vec3> locationBufferData{
    {
      -0.5f, -0.5f, 1.0f,
    }, // Bottom left
    {
      0.5f, -0.5f, 1.0f,
    }, // Bottom right
    {
      -0.5f, 0.5f, 1.0f,
    }, // top left
    {
      0.5f, 0.5f, 1.0f,
    }, // top right
};

vector<vec2> uvFullImage{
    {
      0.0f, 1.0f,
    }, // Bottom left
    {
      1.0f, 1.0f,
    }, // Bottom right
    {
      0.0f, 0.0f,
    }, // Top Left
    {
      1.0f, 0.0f,
    }, // Top right
};

vector<vec2> uvBufferData{
    {
      0.0f, 1.0f,
    },
    {
      1.0f, 1.0f,
    },
    {
      0.0f, 0.0f,
    },
    {
      1.0f, 0.0f,
    },
};

internal_function void
initializeBuffers()
{
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(2, vertexBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, locationBuffer);
    // glBufferData(/*
    //    GL_ARRAY_BUFFER,
    //    sizeof_vector(locationBufferData),
    //    locationBufferData.data(),
    //    GL_STATIC_DRAW);*/

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vector(uvBufferData),
                 uvBufferData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

internal_function GLuint
getUniformLocation(std::string name)
{
    GLuint prog;
    glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&prog);

    return glGetUniformLocation(prog, name.c_str());
}

void
SetUniform(std::string name, const glm::vec4& color)
{
    auto loc = getUniformLocation(name);
    glUniform4f(loc, color.r, color.g, color.b, color.a);
}

void
SetUniform(std::string name, const glm::mat3& value)
{
    auto loc = getUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, (GLfloat*)&value);
}

void
DEBUG_DrawSprite(Sprite spr, glm::mat3 modelView, struct Rectangle sprPart,
                 float rotation)
{
    if (locationBuffer == 0 || uvBuffer == 0) {
        initializeBuffers();
    }

    uvBufferData[0].x = sprPart.Min().x;
    uvBufferData[0].y = sprPart.Max().y;
    uvBufferData[1].x = sprPart.Max().x;
    uvBufferData[1].y = sprPart.Max().y;
    uvBufferData[2].x = sprPart.Min().x;
    uvBufferData[2].y = sprPart.Min().y;
    uvBufferData[3].x = sprPart.Max().x;
    uvBufferData[3].y = sprPart.Min().y;

    glBindVertexArray(vertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vector(uvBufferData),
                 uvBufferData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, locationBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof_vector(locationBufferData),
                 locationBufferData.data(), GL_STATIC_DRAW);

    glBindTexture(GL_TEXTURE_2D, spr.TextureID);

    glUniformMatrix3fv(0, 1, GL_FALSE, (GLfloat*)&modelView);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
