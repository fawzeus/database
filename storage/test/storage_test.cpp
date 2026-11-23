#include <storage/pager.hpp>
#include <cassert>
#include <iostream>

int main() {
    assert(PAGE_SIZE == 4096);
    // TODO(Milestone 5): test pager page read/write, cache, serialization round-trip.
    std::cout << "storage_test: PASSED (constants only)\n";
    return 0;
}
