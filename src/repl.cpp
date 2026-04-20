#include "repl.hpp"
#include <iostream>
namespace repl
{
    void printPrompt() {
        std::cout << "db> ";
    }
    void handleCommand(const std::string& command) {
        if (command == "exit") {
            std::cout << "Exiting REPL..." << std::endl;
            exit(0);
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }
    void loop() {
        std::string command;
        while (true) {
            printPrompt();
            std::getline(std::cin, command);
            handleCommand(command);
        }
    }
    
} // namespace repl
