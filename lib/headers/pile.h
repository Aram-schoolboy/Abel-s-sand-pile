#include "grid.h"

struct SandPileModel {
    Grid* grid;
    long long freq;
    long long max_iter;
    const char* output;
    static const int kMaxOutputLength = 1000;

    SandPileModel(Grid* grid, long long freq, long long max_iter, const char* output);

    void LaunchCollapse() const;
};
