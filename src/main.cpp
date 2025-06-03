#include <iostream>
#include <vector>
#include <string>
#include "task_manager.h"
#include "command_handler.h"

std::vector<std::string> parseArguments(int argc, char* argv[]) {
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
    return args;
}

int main(int argc, char* argv[]) {
    try {
        TaskManager taskManager;
        CommandHandler commandHandler(taskManager);
        
        std::vector<std::string> args = parseArguments(argc, argv);
        
        if (args.empty()) {
            commandHandler.displayHelp();
            return 0;
        }
        
        commandHandler.processCommand(args);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}