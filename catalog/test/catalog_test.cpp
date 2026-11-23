#include <catalog/schema.hpp>
#include <cassert>
#include <iostream>

int main() {
    // Only the skeleton types exist so far (Milestone 2).
    TableSchema schema;
    assert(schema.tableName.empty());
    assert(schema.columns.empty());

    // TODO(Milestone 2): test Catalog::createTable / getTable / tableExists,
    // duplicate-table rejection, and TableSchema::findColumn().
    std::cout << "catalog_test: PASSED (skeleton only)\n";
    return 0;
}
