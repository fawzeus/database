#include <data/row.hpp>
#include <cassert>
#include <iostream>

int main() {
    Row r;
    assert(r.size() == 0);
    assert(!r.hasColumn("id"));

    // all five SQLite value types
    assert(r.addColumn("id", int64_t(1)) == errorId_t::ERR_STATUS_OK);
    assert(r.addColumn("name", std::string("Fawzi")) == errorId_t::ERR_STATUS_OK);
    assert(r.addColumn("score", 9.8) == errorId_t::ERR_STATUS_OK);
    assert(r.addColumn("nothing", std::monostate{}) == errorId_t::ERR_STATUS_OK);
    assert(r.addColumn("blob", std::vector<uint8_t>{1, 2, 3}) == errorId_t::ERR_STATUS_OK);
    assert(r.size() == 5);
    assert(r.addColumn("id", int64_t(2)) == errorId_t::ERR_COLUMN_ALREADY_EXISTS);

    // column order preserved
    size_t count = 0;
    assert(r.getColumnCount(count) == errorId_t::ERR_STATUS_OK && count == 5);
    std::string name;
    assert(r.getColumnName(1, name) == errorId_t::ERR_STATUS_OK && name == "name");
    assert(r.getColumnName(99, name) == errorId_t::ERR_INDEX_OUT_OF_RANGE);
    assert(r.columnNames().size() == 5);

    // lookup by name and index
    Value v;
    assert(r.getValue("name", v) == errorId_t::ERR_STATUS_OK);
    assert(std::get<std::string>(v) == "Fawzi");
    assert(r.get(0, v) == errorId_t::ERR_STATUS_OK && std::get<int64_t>(v) == 1);
    assert(r.get(99, v) == errorId_t::ERR_INDEX_OUT_OF_RANGE);
    assert(r.getValue("nope", v) == errorId_t::ERR_COLUMN_NOT_FOUND);

    // typed getters
    int64_t id = 0;
    double score = 0;
    std::string text;
    assert(r.getAs("id", id) && id == 1);
    assert(r.getAs("score", score) && score == 9.8);
    assert(r.getAs("name", text) && text == "Fawzi");
    assert(!r.getAs("id", text));  // wrong type
    assert(!r.getAs("missing", id));

    // value helpers
    Value textValue = std::string("Fawzi");
    assert(std::string(valueTypeName(textValue)) == "TEXT");
    assert(valueToString(textValue) == "Fawzi");
    Value nv;
    r.getValue("nothing", nv);
    assert(isNull(nv) && valueToString(nv) == "NULL");
    assert(std::string(valueTypeName(nv)) == "NULL");

    // update / remove / equality / clear
    assert(r.setValue("id", int64_t(7)) == errorId_t::ERR_STATUS_OK);
    assert(r.getAs("id", id) && id == 7);
    assert(r.setValue("nope", v) == errorId_t::ERR_COLUMN_NOT_FOUND);
    assert(r.removeColumn("score") == errorId_t::ERR_STATUS_OK);
    assert(r.size() == 4 && !r.hasColumn("score"));
    assert(r.getColumnName(1, name) == errorId_t::ERR_STATUS_OK && name == "name");
    Row copy = r;
    assert(copy == r);
    copy.addColumn("extra", int64_t(0));
    assert(copy != r);
    r.clear();
    assert(r.size() == 0 && r.columnNames().empty());

    // output helpers
    r.addColumn("id", int64_t(1));
    r.addColumn("name", std::string("Fawzi"));
    r.addColumn("blob", std::vector<uint8_t>{1, 2, 3});
    r.print();
    assert(r.toString() == "id: 1 | name: Fawzi | blob: [BLOB of size 3]");

    std::cout << "row_test: PASSED\n";
    return 0;
}
