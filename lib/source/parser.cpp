#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>

#include "../headers/parser.h"

void ShowUsage() {
    std::cout << "Usage: SandPile.exe [OPTION]\n"
              << "Options:\n"
              << "\t-i,--input=path\t\ttsf-file path\n"
              << "\t-o,--output=path\tpath for saving images\n"
              << "\t-m,--max-iter=n\t\tmaximum number of iterations\n"
              << "\t-f,--freq=k\t\tfrequency of saving images\n"
              << std::endl;
}

bool FileExists(const char* filename) {
    std::ifstream file(filename);
    return file.good();
}

bool DirectoryExists(const char* directory) {
    return (std::filesystem::exists(directory) && (is_directory(std::filesystem::status(directory))));
}

bool StartsWith(const char* text, const char* part) {
    for (int i = 0; i < std::strlen(part); ++i) {
        if (text[i] != part[i] || text[i] == '\0') {
            return false;
        }
    }

    return true;
}

long long ProcessNumberInput(const char* input) {
    errno = 0;
    char* p_end;
    long long number = std::strtoll(input, &p_end, 10);
    if ((*p_end != '\0') || (number < 0) || (errno == ERANGE)) {
        number = -1;
    }

    return number;
}

bool Options::CheckValidityOfArgs() const {
    if (!FileExists(input)) {
        std::cerr << "Invalid input" << std::endl;
        return false;
    } if (max_iter < 0) {
        std::cerr << "Invalid max-iter" << std::endl;
        return false;
    } if (freq < 0) {
        std::cerr << "Invalid freq" << std::endl;
        return false;
    } if (!DirectoryExists(output)) {
        std::cerr << "Invalid output" << std::endl;
        return false;
    }

    return true;
}

void FillNames(Argument& freq, Argument& max_iter, Argument& input, Argument& output) {
    freq.short_name = "-f";
    freq.long_name = "--freq=";
    max_iter.short_name = "-m";
    max_iter.long_name = "--max-iter=";
    input.short_name = "-i";
    input.long_name = "--input=";
    output.short_name = "-o";
    output.long_name = "--output=";
}

Options ParseArguments(int argc, char** argv) {
    Options options;
    Argument freq;
    Argument max_iter;
    Argument input;
    Argument output;
    FillNames(freq, max_iter, input, output);

    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];
        if (strcmp(arg, freq.short_name) == 0) {
            if (++i >= argc) {
                options.freq = -1;
                std::cerr << "frequency value is not defined" << std::endl;
                return options;
            }
            char* freq_input = argv[i];
            long long freq_value = ProcessNumberInput(freq_input);
            options.freq = freq_value;
        } else if (StartsWith(arg, freq.long_name)) {
            char* freq_input = arg + std::strlen(freq.long_name);
            long long freq_value = ProcessNumberInput(freq_input);
            options.freq = freq_value;
        } else if (strcmp(arg, max_iter.short_name) == 0) {
            if (++i >= argc) {
                options.max_iter = -1;
                std::cerr << "max-iter value is not defined" << std::endl;
                return options;
            }
            char* max_iter_input = argv[i];
            long long max_iter_value = ProcessNumberInput(max_iter_input);
            options.max_iter = max_iter_value;
        } else if (StartsWith(arg, max_iter.long_name)) {
            char* max_iter_input = arg + std::strlen(max_iter.long_name);
            long long max_iter_value = ProcessNumberInput(max_iter_input);
            options.max_iter = max_iter_value;
        } else if (strcmp(arg, input.short_name) == 0) {
            if (++i >= argc) {
                std::cerr << "input path is not defined" << std::endl;
                return options;
            }
            char* input_path = argv[i];
            options.input = input_path;
        } else if (StartsWith(arg, input.long_name)) {
            char* input_path = arg + std::strlen(input.long_name);
            options.input = input_path;
        } else if (strcmp(arg, output.short_name) == 0) {
            if (++i >= argc) {
                std::cerr << "output path is not defined" << std::endl;
                return options;
            }
            char* output_path = argv[i];
            options.output = output_path;
        } else if (StartsWith(arg, output.long_name)) {
            char* output_path = arg + std::strlen(output.long_name);
            options.output = output_path;
        }
    }

    return options;
}