#ifndef SCHEMA_HPP
#define SCHEMA_HPP

#include <string>
#include <vector>

// STATUS: skeleton types — behavior arrives in Milestone 2.
// Responsibility: describe table structure (columns, types, constraints).

enum class ColumnType {
    NULL_TYPE,
    INTEGER,
    REAL,
    TEXT,
    BLOB
};

struct ColumnDef {
    std::string name;
    ColumnType type = ColumnType::TEXT;
    bool notNull = false;
    bool primaryKey = false;
};

struct TableSchema {
    std::string tableName;
    std::vector<ColumnDef> columns;
    // TODO(Milestone 2): int findColumn(const std::string& name) const;
};

#endif /* SCHEMA_HPP */
