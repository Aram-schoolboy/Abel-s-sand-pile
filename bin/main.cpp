#include "lib/headers/parser.h"
#include "lib/headers/pile.h"

int main(int argc, char** argv) {
    Options options = ParseArguments(argc, argv);
    if (!options.CheckValidityOfArgs()) {
        ShowUsage();
        return 1;
    }

    Grid grid = MakeGridFromFile(options.input);
    SandPileModel sand_pile = SandPileModel(&grid, options.freq, options.max_iter, options.output);
    sand_pile.LaunchCollapse();

    return 0;
}
