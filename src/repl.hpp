#include <string>
namespace repl
{
    void loop();
    void printPrompt();
    void handleCommand(const std::string& command);
} // namespace repl
