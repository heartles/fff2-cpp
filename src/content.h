#pragma once

#include <cstdlib>
#include <string>
#include <glad/glad.h>

std::string
ReadAllText(const std::string filename);

size_t
GetFileSize(std::FILE *file);
