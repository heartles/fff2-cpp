#pragma once

#include <cstdlib>
#include <glad/glad.h>
#include <string>

std::string ReadAllText(const std::string filename);

size_t GetFileSize(std::FILE* file);
