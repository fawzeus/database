#include <sql/sqlstatement.hpp>
#include <cassert>
#include <iostream>

int main() {
    Sqlstatement statement;

    assert(statement.prepareStatement("SELECT * FROM users") == errorId_t::ERR_STATUS_OK);
    assert(statement.excuteCommand() == errorId_t::ERR_STATUS_OK);

    assert(statement.prepareStatement("INSERT INTO users VALUES (1)") == errorId_t::ERR_STATUS_OK);
    assert(statement.excuteCommand() == errorId_t::ERR_STATUS_OK);

    assert(statement.prepareStatement("UPDATE users SET id = 2") == errorId_t::ERR_INVALID_STATEMENT_TYPE);

    std::cout << "sqlstatement_test: PASSED\n";
    return 0;
}
