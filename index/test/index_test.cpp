#include <index/btree.hpp>
#include <cassert>
#include <iostream>

int main() {
    assert(INTERNAL_NODE == 0);
    assert(LEAF_NODE == 1);
    // TODO(Milestone 6): test leaf insert/search, root split, cursor advance.
    std::cout << "index_test: PASSED (constants only)\n";
    return 0;
}
