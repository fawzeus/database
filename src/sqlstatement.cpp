#include "sqlstatement.hpp"

errorId_t Sqlstatement::prepareStatement(const std::string& command) {
    errorId_t status = errorId_t::ERR_STATUS_OK;
    if (command.compare(0, 6, "SELECT") == 0) {
        type = statementType::STATEMENT_TYPE_SELECT;
    } else if (command.compare(0, 6, "INSERT") == 0) {
        type = statementType::STATEMENT_TYPE_INSERT;
    } else {
        status = errorId_t::ERR_INVALID_STATEMENT_TYPE;
    }
    return status;
}

errorId_t Sqlstatement::excuteCommand() {
    errorId_t status = errorId_t::ERR_STATUS_OK;
    switch (type) {
        case statementType::STATEMENT_TYPE_INSERT:
            std::cout<<"This is an INSERT command"<<"\n";
            break;
        case statementType::STATEMENT_TYPE_SELECT:
            std::cout<<"This is a SELECT command"<<"\n";
            break;
        default:
            break;
    }
    return status;
}