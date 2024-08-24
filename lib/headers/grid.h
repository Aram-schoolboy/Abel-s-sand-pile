#include <cinttypes>

#ifndef GRID_H
#define GRID_H

#pragma pack(push, 1)
struct Cell {
    uint64_t value = 0;
    bool is_stable = true;

    void SetValue(uint64_t number);

    void SetStability();
};
#pragma pack(pop)

struct Row {
    Cell* cells;

    Row();

    Row(uint32_t row_size);

    void ExpandRight(uint32_t size, uint32_t right_expand_number);

    void ExpandLeft(uint32_t size, uint32_t left_expand_number);
};

struct Grid {
    uint32_t bottom_expand_number;
    uint32_t top_expand_number;
    uint32_t right_expand_number;
    uint32_t left_expand_number;
    uint32_t row_count;
    uint32_t row_size;
    uint32_t bottom_max;
    uint32_t top_max;
    uint32_t right_max;
    uint32_t left_max;
    bool increment_bottom_max = false;
    bool increment_top_max = false;
    bool increment_right_max = false;
    bool increment_left_max = false;
    Row* rows;

    Grid(uint32_t row_count, uint32_t row_size);

    void ExpandRight();

    void ExpandLeft();

    void CopyRow(const Row& original_row, const Row& final_row) const;

    void ExpandBottom();

    void ExpandTop();

    void Expand();

    void MakeCollapse(uint32_t i, uint32_t j) const;

    bool CellIsStable(uint32_t i, uint32_t j) const;

    bool AnyUnstableOnVerticalLine(uint32_t column) const;

    bool AnyUnstableOnHorizontalLine(uint32_t row) const;

    uint64_t GetCellValue(uint32_t i, uint32_t j) const;

    void SetBoardsIncrementNecessity();

    void ToIncrementBoards();

    void SetCellsStability() const;

    bool MakeNaiveIteration();
};

Grid MakeGridFromFile(const char* input);

#endif