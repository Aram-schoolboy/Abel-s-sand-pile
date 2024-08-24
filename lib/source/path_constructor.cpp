#include <cstring>
#include <cmath>
#include <charconv>

#include "../headers/path_constructor.h"

int GetLengthOfNumber(long long number) {
    return number == 0 ? 1 : static_cast<int>(log10(number)) + 1;
}

void MakeFullPath(char* res, const char* directory_folder, const long long number) {
    static const char* kExtension = ".bmp";
    static const char* kSlash = "\\";
    int length_of_number = GetLengthOfNumber(number);
    int res_pointer = 0;
    std::strcat(res, directory_folder);
    std::strcat(res, kSlash);
    res_pointer += std::strlen(directory_folder) + std::strlen(kSlash);
    std::to_chars(res + res_pointer, res + res_pointer + length_of_number, number);
    std::strcat(res, kExtension);
}

