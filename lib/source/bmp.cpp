#include <iostream>
#include <fstream>

#include "../headers/bmp.h"

void WriteFileHeader(std::ofstream& bmp, const uint32_t width, const uint32_t height, const uint8_t padding) {
    uint8_t file_header[kFileHeaderSize] = {};
    uint32_t size = kFileHeaderSize + kInfoHeaderSize + kPaletteSize + ((width + 1) / 2 + padding) * height;
    uint8_t bytes_before_pixels = kFileHeaderSize + kInfoHeaderSize + kPaletteSize;
    file_header[0] = 'B';
    file_header[1] = 'M';
    file_header[2] = static_cast<uint8_t>(size);
    file_header[3] = static_cast<uint8_t>(size >> 8);
    file_header[4] = static_cast<uint8_t>(size >> 16);
    file_header[5] = static_cast<uint8_t>(size >> 24);
    file_header[10] = bytes_before_pixels;
    bmp.write(reinterpret_cast<char*>(file_header), kFileHeaderSize);
}

void WriteInfoHeader(std::ofstream& bmp, const uint32_t width, const uint32_t height) {
    uint8_t info_header[kInfoHeaderSize] = {};
    info_header[0] = kInfoHeaderSize;
    info_header[4] = static_cast<uint8_t>(width);
    info_header[5] = static_cast<uint8_t>(width >> 8);
    info_header[6] = static_cast<uint8_t>(width >> 16);
    info_header[7] = static_cast<uint8_t>(width >> 24);
    info_header[8] = static_cast<uint8_t>(height);
    info_header[9] = static_cast<uint8_t>(height >> 8);
    info_header[10] = static_cast<uint8_t>(height >> 16);
    info_header[11] = static_cast<uint8_t>(height >> 24);
    info_header[12] = 1;
    info_header[14] = kBitsPerPixel;
    info_header[32] = kNumberOfColors;
    bmp.write(reinterpret_cast<char*>(info_header), kInfoHeaderSize);
}

void WritePallet(std::ofstream& bmp) {
    static const uint8_t white[kColorComponentsNumber] = {255, 255, 255, 0};
    static const uint8_t green[kColorComponentsNumber] = {0, 128, 0, 0};
    static const uint8_t yellow[kColorComponentsNumber] = {0, 255, 255, 0};
    static const uint8_t purple[kColorComponentsNumber] = {255, 0, 128, 0};
    static const uint8_t black[kColorComponentsNumber] = {0, 0, 0, 0};
    static const uint8_t* colors[kNumberOfColors] = {white, green, yellow, purple, black};

    for (int i = 0; i < kNumberOfColors; ++i) {
        for (int j = 0; j < kColorComponentsNumber; ++j) {
            bmp << colors[i][j];
        }
    }
}

void WritePadding(std::ofstream& bmp, const uint8_t padding) {
    for (int k = 0; k < padding; ++k) {
        bmp << 0;
    }
}

void WriteTwoPixels(std::ofstream& bmp, const uint8_t pixel1, const uint8_t pixel2) {
    uint8_t byte = 0;
    byte += pixel1 << kBitsPerPixel;
    byte += pixel2;
    bmp << byte;
}

void WritePixels(std::ofstream& bmp, const Grid& grid, const uint8_t padding) {
    for (int i = grid.top_max; i <= grid.bottom_max; ++i) {
        int reversed_i = grid.bottom_max - (i - grid.top_max);
        for (int j = grid.left_max; j <= grid.right_max; j += 2) {
            uint8_t pixel1 = kNumberOfColors - 1;
            if (pixel1 > grid.GetCellValue(reversed_i, j)) {
                pixel1 = grid.GetCellValue(reversed_i, j);
            } if (j == grid.right_max) {
                WriteTwoPixels(bmp, pixel1, 0);
                continue;
            }

            uint8_t pixel2 = kNumberOfColors - 1;
            if (pixel2 > grid.GetCellValue(reversed_i, j + 1)) {
                pixel2 = grid.GetCellValue(reversed_i, j + 1);
            }
            WriteTwoPixels(bmp, pixel1, pixel2);
        }
        WritePadding(bmp, padding);
    }
}

void MakeBmpFromGrid(const Grid& grid, const char* output) {
    const uint32_t width = grid.right_max - grid.left_max + 1;
    const uint32_t height = grid.bottom_max - grid.top_max + 1;
    const uint8_t padding = (4 - (((width + 1) / 2) % 4)) % 4;

    std::ofstream bmp(output, std::ios::out | std::ios::binary);
    WriteFileHeader(bmp, width, height, padding);
    WriteInfoHeader(bmp, width, height);
    WritePallet(bmp);
    WritePixels(bmp, grid, padding);
    bmp.close();
}
