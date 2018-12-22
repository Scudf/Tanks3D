#pragma once

#include <memory>


struct PngTexData
{
	unsigned int width;
	unsigned int height;

	std::shared_ptr<char> data;

	bool alphaChannel;
};

bool loadPng(const char* path, PngTexData& result);


