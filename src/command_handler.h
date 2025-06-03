#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <vector>
#include <string>
#include "task_manager.h"

class CommandHandler {
private:
    TaskManager& taskManager;

public:
    explicit CommandHandler(TaskManager& tm);

    // Command handlers
    void handleAdd(const std::vector<std::string>& args);
    void handleUpdate(const std::vector<std::string>& args);
    void handleDelete(const std::vector<std::string>& args);
    void handleList(const std::vector<std::string>& args);
    void handleDone(const std::vector<std::string>& args);
    void handleProgress(const std::vector<std::string>& args);
    
    // Utility
    void displayHelp();
    void displayTasks(const std::vector<Task>& tasks, const std::string& title = "") const;
    
    // Main command processor
    void processCommand(const std::vector<std::string>& args);
};

#endif