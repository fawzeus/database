#ifndef BTREE_HPP
#define BTREE_HPP

#include <cstdint>

// STATUS: placeholder — implement in Milestone 6.
// Responsibility: B+ tree layout and operations over pager pages.
// Internal nodes route by key; linked leaf nodes hold the row data.

constexpr uint8_t INTERNAL_NODE = 0;
constexpr uint8_t LEAF_NODE = 1;

// TODO(Milestone 6), implement in stages:
//   bool btreeSearch(uint32_t rootPage, int64_t key);
//   void btreeInsert(uint32_t rootPage, int64_t key, const Row& row);
//   void splitChild(...);   // root split first, then general splits

#endif /* BTREE_HPP */
