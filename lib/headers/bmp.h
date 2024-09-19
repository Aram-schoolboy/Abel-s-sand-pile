#include <iostream>
#include <fstream>

#include "grid.h"

static const uint8_t kFileHeaderSize = 14;
static const uint8_t kInfoHeaderSize = 40;
static const uint8_t kPaletteSize = 20;
static const uint8_t kBitsPerPixel = 4;
static const uint8_t kNumberOfColors = 5;
static const uint8_t kColorComponentsNumber = 4;

void WriteUint32InBytesArray(uint8_t* array, size_t index, uint32_t value);

void WriteFileHeader(std::ofstream& bmp, uint32_t width, uint32_t height, uint8_t padding);

void WriteInfoHeader(std::ofstream& bmp, uint32_t width, uint32_t height);

void WritePallet(std::ofstream& bmp);

void WritePadding(std::ofstream& bmp, uint8_t padding);

void WriteTwoPixels(std::ofstream& bmp, uint8_t pixel1, uint8_t pixel2);

void WritePixels(std::ofstream& bmp, const Grid& grid, uint8_t padding);

void MakeBmpFromGrid(const Grid& grid, const char* output);
