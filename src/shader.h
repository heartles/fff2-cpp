#pragma once

#include <string>

#include <glad/glad.h>

struct Shader
{
    GLuint _vertShader, _fragShader, _program;

    void Apply();
};

Shader LoadShader(std::string vertPath, std::string fragPath);
