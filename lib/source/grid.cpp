#include <iostream>
#include <fstream>

#include "../headers/grid.h"

void Cell::SetValue(const uint64_t number) {
    value = number;
    SetStability();
}

void Cell::SetStability() {
    is_stable = (value <= 3);
}

Row::Row() = default;

Row::Row(const uint32_t row_size) {
    cells = new Cell[row_size];
}

void Row::ExpandRight(const uint32_t size, const uint32_t right_expand_number) {
    Cell* temp = new Cell[size + right_expand_number];
    for (int i = 0; i < size; ++i) {
        temp[i] = cells[i];
    }
    delete[] cells;
    cells = temp;
}

void Row::ExpandLeft(const uint32_t size, const uint32_t left_expand_number) {
    Cell* temp = new Cell[size + left_expand_number];
    for (int i = left_expand_number; i < left_expand_number + size; ++i) {
        temp[i] = cells[i - left_expand_number];
    }
    delete[] cells;
    cells = temp;
}

Grid::Grid(const uint32_t row_count, const uint32_t row_size) {
    this->row_count = row_count;
    this->row_size = row_size;
    rows = new Row[row_count];
    for (int i = 0; i < row_count; ++i) {
        rows[i] = Row(row_size);
    }
    top_max = 0;
    bottom_max = row_count - 1;
    left_max = 0;
    right_max = row_size - 1;
    bottom_expand_number = row_count / 2 + 1;
    top_expand_number = bottom_expand_number;
    right_expand_number = row_size / 2 + 1;
    left_expand_number = right_expand_number;
}

void Grid::ExpandRight() {
    for (int i = 0; i < row_count; ++i) {
        rows[i].ExpandRight(row_size, right_expand_number);
    }
    row_size += right_expand_number;
    right_expand_number *= 2;
}

void Grid::ExpandLeft() {
    for (int i = 0; i < row_count; ++i) {
        rows[i].ExpandLeft(row_size, left_expand_number);
    }
    row_size += left_expand_number;
    left_max += left_expand_number;
    right_max += left_expand_number;
    left_expand_number *= 2;
}

void Grid::CopyRow(const Row &original_row, const Row &final_row) const {
    for (int i = 0; i < row_size; ++i) {
        final_row.cells[i] = original_row.cells[i];
    }
}

void Grid::ExpandBottom() {
    Row* temp = new Row[row_count + bottom_expand_number];
    for (int i = 0; i < row_count; ++i) {
        temp[i] = Row(row_size);
        CopyRow(rows[i], temp[i]);
    }
    for (int i = row_count; i < row_count + bottom_expand_number; ++i) {
        temp[i] = Row(row_size);
    }
    for (int i = 0; i < row_count; ++i) {
        delete[] rows[i].cells;
    }
    delete[] rows;
    rows = temp;
    row_count += bottom_expand_number;
    bottom_expand_number *= 2;
}

void Grid::ExpandTop() {
    Row* temp = new Row[row_count + top_expand_number];
    for (int i = 0; i < top_expand_number; ++i) {
        temp[i] = Row(row_size);
    }
    for (int i = top_expand_number; i < row_count + top_expand_number; ++i) {
        temp[i] = Row(row_size);;
        CopyRow(rows[i - top_expand_number], temp[i]);
    }
    for (int i = 0; i < row_count; ++i) {
        delete[] rows[i].cells;
    }
    delete[] rows;
    rows = temp;
    row_count += top_expand_number;
    top_max += top_expand_number;
    bottom_max += top_expand_number;
    top_expand_number *= 2;
}

void Grid::Expand() {
    if (left_max == 0) {
        ExpandLeft();
    } if (right_max == row_size - 1) {
        ExpandRight();
    } if (bottom_max == row_count - 1) {
        ExpandBottom();
    } if (top_max == 0) {
        ExpandTop();
    }
}

void Grid::MakeCollapse(const uint32_t i, const uint32_t j) const {
    rows[i].cells[j].value -= 4;
    rows[i+1].cells[j].value += 1;
    rows[i].cells[j+1].value += 1;
    rows[i-1].cells[j].value += 1;
    rows[i].cells[j-1].value += 1;
}

bool Grid::CellIsStable(const uint32_t i, const uint32_t j) const {
    return rows[i].cells[j].is_stable;
}

bool Grid::AnyUnstableOnVerticalLine(const uint32_t column) const {
    for (int i = top_max; i <= bottom_max; ++i) {
        if (!CellIsStable(i, column)) {
            return true;
        }
    }
    return false;
}

bool Grid::AnyUnstableOnHorizontalLine(const uint32_t row) const {
    for (int j = left_max; j <= right_max; ++j) {
        if (!CellIsStable(row, j)) {
            return true;
        }
    }
    return false;
}

uint64_t Grid::GetCellValue(const uint32_t i, const uint32_t j) const {
    return rows[i].cells[j].value;
}

void Grid::SetBoardsIncrementNecessity() {
    increment_top_max = AnyUnstableOnHorizontalLine(top_max);
    increment_bottom_max = AnyUnstableOnHorizontalLine(bottom_max);
    increment_left_max = AnyUnstableOnVerticalLine(left_max);
    increment_right_max = AnyUnstableOnVerticalLine(right_max);
}

void Grid::ToIncrementBoards() {
    if (increment_left_max) {
        left_max--;
    } if (increment_right_max) {
        right_max++;
    } if (increment_top_max) {
        top_max--;
    } if (increment_bottom_max) {
        bottom_max++;
    }
}

void Grid::SetCellsStability() const {
    for (int i = top_max; i <= bottom_max; ++i) {
        for (int j = left_max; j <= right_max; ++j) {
            rows[i].cells[j].SetStability();
        }
    }
}

bool Grid::MakeNaiveIteration() {
    bool has_changed = false;
    Expand();
    SetBoardsIncrementNecessity();
    for (int i = top_max; i <= bottom_max; ++i) {
        for (int j = left_max; j <= right_max; ++j) {
            if (!CellIsStable(i, j)) {
                MakeCollapse(i, j);
                has_changed = true;
            }
        }
    }
    ToIncrementBoards();
    SetCellsStability();
    return has_changed;
}

Grid MakeGridFromFile(const char* input) {
    std::ifstream fin(input);
    int16_t x;
    int16_t y;
    uint64_t value;
    fin >> x >> y >> value;
    int16_t max_x = x;
    int16_t min_x = x;
    int16_t max_y = y;
    int16_t min_y = y;
    while (fin >> x >> y >> value) {
        max_x = std::max(x, max_x);
        min_x = std::min(x, min_x);
        max_y = std::max(y, max_y);
        min_y = std::min(y, min_y);
    }
    fin.close();

    uint32_t row_size = max_x - min_x + 1;
    uint32_t row_count = max_y - min_y + 1;
    Grid grid = Grid(row_count, row_size);
    fin.open(input);
    while (fin >> x >> y >> value) {
        grid.rows[max_y - y].cells[x - min_x].SetValue(value);
    }

    return grid;
}
