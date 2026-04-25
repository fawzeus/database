#include "repl.hpp"
#include <iostream>
#include "metacommand.hpp"
#include "sqlstatement.hpp"
#include "utils.hpp"
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
            Sqlstatement statement;
            printPrompt();
            std::getline(std::cin, command);
            if (command[0] == '.') {
                switch (handleMetaCommand(command))
                {
                case errorId_t::ERR_STATUS_OK:
                    continue;                
                default:
                    std::cout<<"Unrecognized command "<<command<<"\n";
                    continue;
                }
            }
            switch (statement.prepareStatement(command)) {
                case errorId_t::ERR_STATUS_OK:
                    break;
                default:
                    std::cout<<"Unrecognized keyword at start of "<<command<<"\n";
                    continue;;
            }
            statement.excuteCommand();
            std::cout<<"COMMAND EXECUTED!"<<"\n";
        }
    }
    
} // namespace repl
