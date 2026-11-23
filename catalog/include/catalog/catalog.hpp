#ifndef CATALOG_HPP
#define CATALOG_HPP

#include <map>
#include <string>
#include <catalog/schema.hpp>

// STATUS: placeholder — implement in Milestone 2.
// Responsibility: registry of all table schemas (and later, table objects).
class Catalog {
public:
    bool createTable(const TableSchema& schema);
    const TableSchema* getTable(const std::string& name) const;
    bool tableExists(const std::string& name) const;

private:
    std::map<std::string, TableSchema> tables_;
};

#endif /* CATALOG_HPP */
