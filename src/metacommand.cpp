#include "metacommand.hpp"

errorId_t handleMetaCommand(const std::string& input) {
    if (input == ".exit") {
        std::exit(0);
    } else {
        return errorId_t::ERR_INVALID_COMMAND;
    }
}