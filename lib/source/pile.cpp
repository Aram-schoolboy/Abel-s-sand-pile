#include "../headers/pile.h"
#include "../headers/path_constructor.h"
#include "../headers/bmp.h"

SandPileModel::SandPileModel(Grid* grid, long long freq, long long max_iter, const char* output) {
    this->grid = grid;
    this->max_iter = max_iter;
    this->output = output;
    this->freq = freq;
    if (freq == 0) {
        this->freq = max_iter + 1;
    }
}

void SandPileModel::LaunchCollapse() const {
    long long iter_count = 0;
    while ((iter_count < max_iter) && ((*grid).MakeNaiveIteration())) {
        iter_count++;
        if (iter_count % freq == 0) {
            char full_path[kMaxOutputLength] {};
            MakeFullPath(full_path, output, iter_count);
            MakeBmpFromGrid(*grid, full_path);
        }
    }

    if (iter_count % freq != 0 || iter_count == 0) {
        char full_path[kMaxOutputLength] {};
        MakeFullPath(full_path, output, iter_count);
        MakeBmpFromGrid(*grid, full_path);
    }
}
