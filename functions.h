#pragma once
#include "Image.h"
#include <stdint.h>
#include <Windows.h>
#include <fstream>
#include <string>
Image readImage(std::string filename);

void DisplayImage(const Image& img);

void writeImage(const std::string& filename, const Image& image);