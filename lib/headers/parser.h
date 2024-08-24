void ShowUsage();

bool FileExists(const char* filename);

bool DirectoryExists(const char* directory);

bool StartsWith(const char* text, const char* part);

long long ProcessNumberInput(const char* input);

struct Options {
    char* input = nullptr;
    char* output = nullptr;
    long long max_iter = 0;
    long long freq = 0;

    bool CheckValidityOfArgs() const;
};

struct Argument {
    const char* short_name = nullptr;
    const char* long_name = nullptr;
};

void FillNames(Argument& freq, Argument& max_iter, Argument& input, Argument& output);

Options ParseArguments(int argc, char** argv);
