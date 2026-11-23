#include <core/repl.hpp>
#include <data/row.hpp>
int main() {
    repl::loop();
/*    Row row;
    row.addColumn("id", int64_t(1));
    row.addColumn("name", std::string("Alice"));
    row.addColumn("age", int64_t(30));
    row.addColumn("salary", double(50000.0));
    row.print();
    return 0;
*/
}
