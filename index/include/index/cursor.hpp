#ifndef CURSOR_HPP
#define CURSOR_HPP

#include <cstdint>

class Table;  // defined in data/src/table.hpp

// STATUS: placeholder — implement in Milestone 6.
// Responsibility: point at one row during a table scan or an index seek.
struct Cursor {
    Table* table = nullptr;
    uint32_t pageNum = 0;
    uint32_t cellNum = 0;
    bool endOfTable = true;
};

Cursor tableStart(Table* table);
Cursor tableFind(Table* table, int64_t key);
void cursorAdvance(Cursor& cursor);

#endif /* CURSOR_HPP */
