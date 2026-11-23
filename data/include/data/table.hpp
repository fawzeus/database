#ifndef TABLE_HPP
#define TABLE_HPP

#include <cstddef>
#include <vector>
#include <data/row.hpp>
#include <catalog/schema.hpp>

// STATUS: placeholder — implement in Milestone 3.
// Responsibility: hold rows for one table. Initially a std::vector<Row>;
// in Milestone 5 this becomes pager-backed pages.
class Table {
public:
    explicit Table(const TableSchema& schema);

    bool insert(const Row& row);
    const std::vector<Row>& rows() const;
    const TableSchema& schema() const;
    std::size_t rowCount() const;

private:
    TableSchema schema_;
    std::vector<Row> rows_;
};

#endif /* TABLE_HPP */
