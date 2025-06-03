#include <iostream>
#include <vector>
#include <string>
#include "task_manager.h"
#include "command_handler.h"
#include "interactive_mode.h"

std::vector<std::string> parseArguments(int argc, char *argv[])
{
    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.emplace_back(argv[i]);
    }
    return args;
}

bool isInteractiveMode(const std::vector<std::string> &args)
{
    if (args.empty())
        return false;
    return (args[0] == "interactive" || args[0] == "-i" || args[0] == "--interactive");
}

int main(int argc, char *argv[])
{
    try
    {
        TaskManager taskManager;
        std::vector<std::string> args = parseArguments(argc, argv);

        // Check for interactive mode
        if (isInteractiveMode(args))
        {
            InteractiveMode interactiveMode(taskManager);
            interactiveMode.start();
            return 0;
        }

        // Regular command mode
        CommandHandler commandHandler(taskManager);

        if (args.empty())
        {
            commandHandler.displayHelp();
            return 0;
        }

        commandHandler.processCommand(args);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}