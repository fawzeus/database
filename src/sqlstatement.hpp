#ifndef SQL_STATEMENT_HPP
#define SQL_STATEMENT_HPP
#include <string>
#include <iostream>
#include "sqlstatement.hpp"
#include "utils.hpp"

enum class statementType {
    STATEMENT_TYPE_INSERT,
    STATEMENT_TYPE_SELECT
};
class Sqlstatement {
private:
    statementType type;
public:
    Sqlstatement(){};
    errorId_t prepareStatement(const std::string& command);
    errorId_t excuteCommand();
    ~Sqlstatement(){};
};

#endif /* SQL_STATEMENT_HPP */