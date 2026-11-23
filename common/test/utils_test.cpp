#include <common/utils.hpp>
#include <cassert>
#include <iostream>
#include <set>

int main() {
    // Error codes must stay distinct, otherwise switch-based dispatch breaks.
    std::set<int> codes;
    for (int code = static_cast<int>(errorId_t::ERR_STATUS_OK);
         code <= static_cast<int>(errorId_t::ERR_DATABASE_UNKNOWN_ERROR); ++code) {
        codes.insert(code);
    }
    assert(codes.size() ==
           static_cast<int>(errorId_t::ERR_DATABASE_UNKNOWN_ERROR) + 1);

    std::cout << "utils_test: PASSED\n";
    return 0;
}
