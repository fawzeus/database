#ifndef ROW_HPP
#define ROW_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include <common/utils.hpp>

// A single cell value: NULL, INTEGER, REAL, TEXT or BLOB.
using Value = std::variant<
    std::monostate,       // NULL
    int64_t,              // INTEGER
    double,               // REAL
    std::string,          // TEXT
    std::vector<uint8_t>  // BLOB
>;

// Shared by print(), and later by the parser, serializer and WHERE evaluation.
const char* valueTypeName(const Value& value);
std::string valueToString(const Value& value);
bool isNull(const Value& value);

class Row {
public:
    Row() = default;
    ~Row() = default;

    // Add a new column. Fails if the column already exists.
    errorId_t addColumn(const std::string& columnName, const Value& value);

    // Change an existing column's value. Fails if the column does not exist.
    errorId_t setValue(const std::string& columnName, const Value& value);

    // Remove a column, keeping column order consistent.
    errorId_t removeColumn(const std::string& columnName);

    // Lookup by name or by zero-based insertion index.
    errorId_t getValue(const std::string& columnName, Value& value) const;
    errorId_t get(size_t index, Value& value) const;

    // Typed lookup: returns false on missing column or type mismatch.
    template <typename T>
    bool getAs(const std::string& columnName, T& out) const {
        Value value;
        if (getValue(columnName, value) != errorId_t::ERR_STATUS_OK) {
            return false;
        }
        const T* found = std::get_if<T>(&value);
        if (found == nullptr) {
            return false;
        }
        out = *found;
        return true;
    }

    template <typename T>
    bool getAs(size_t index, T& out) const {
        Value value;
        if (get(index, value) != errorId_t::ERR_STATUS_OK) {
            return false;
        }
        const T* found = std::get_if<T>(&value);
        if (found == nullptr) {
            return false;
        }
        out = *found;
        return true;
    }

    bool hasColumn(const std::string& columnName) const;

    size_t size() const;
    errorId_t getColumnCount(size_t& count) const;
    errorId_t getColumnName(size_t index, std::string& columnName) const;
    const std::vector<std::string>& columnNames() const;
    void clear();

    // Output helpers.
    void print() const;                  // one "column: value" line per column, to stdout
    void print(std::ostream& os) const;  // same format, to any stream
    std::string toString() const;        // compact: "id: 1 | name: Fawzi"

    bool operator==(const Row& other) const;
    bool operator!=(const Row& other) const;

private:
    std::map<std::string, Value> data_;
    std::vector<std::string> columnOrder_;  // preserves insertion order
};

#endif /* ROW_HPP */
