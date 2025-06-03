#ifndef INTERACTIVE_MODE_H
#define INTERACTIVE_MODE_H

#include <string>
#include <vector>
#include "task_manager.h"
#include "command_handler.h"

class InteractiveMode
{
private:
    TaskManager &taskManager;
    CommandHandler commandHandler;
    bool running;
    int sessionCommands;

public:
    explicit InteractiveMode(TaskManager &tm);

    // Main control methods
    void start();
    void stop();
    bool isRunning() const;

    // Display methods
    void displayWelcome();
    void displayPrompt();
    void displayGoodbye();
    void displayStats();

    // Input processing
    std::vector<std::string> parseInput(const std::string &input);
    void processCommand(const std::vector<std::string> &args);

    // Session management
    void autoSave();
    void clearScreen();
    void showHelp();

private:
    // Helper methods
    std::string trim(const std::string &str);
    std::vector<std::string> tokenize(const std::string &input);
    bool isExitCommand(const std::string &command);
    void handleSpecialCommands(const std::vector<std::string> &args);
};

#endif