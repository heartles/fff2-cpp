#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <stb/image.h>
#include <cstdlib>
#include "common.h"
#include <string>
#include <glad/glad.h>
#include "content.h"
#include "game.h"

size_t
GetFileSize(std::FILE *file)
{
	auto save = std::ftell(file);

	std::fseek(file, 0, SEEK_END);
	auto ret = std::ftell(file);

	std::fseek(file, save, SEEK_SET);

	return ret;
}

std::string
ReadAllText(std::string filename)
{
    Log(filename);
	auto file = std::fopen(filename.c_str(), "rb");
	if (!file) {
		//TODO
		Log("error reading file " + filename);
		std::exit(-1);
	}

	size_t fileSize = GetFileSize(file);
	auto buf = new char[fileSize];

	auto res = std::fread(buf, 1, fileSize, file);
	if (res != fileSize) {
		Log("error reading file " + filename);
		std::exit(-1);
	}

	//Add our null terminated byte
	((char *)buf)[fileSize] = 0;

	std::fclose(file);
	Log("2");
	return (char *)buf;

}

