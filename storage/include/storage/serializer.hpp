#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <cstdint>
#include <vector>
#include <data/row.hpp>
#include <catalog/schema.hpp>

// STATUS: placeholder — implement in Milestone 5.
// Responsibility: convert Row <-> raw bytes for page storage.
// Planned format per value: 1-byte type tag, optional 4-byte length, payload.
std::vector<uint8_t> serializeRow(const Row& row, const TableSchema& schema);
Row deserializeRow(const uint8_t* data, const TableSchema& schema);

#endif /* SERIALIZER_HPP */
