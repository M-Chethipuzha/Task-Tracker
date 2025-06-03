#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <vector>
#include <string>
#include "task_manager.h"

class CommandHandler
{
private:
    TaskManager &taskManager;

public:
    explicit CommandHandler(TaskManager &tm);

    // Basic command handlers
    void handleAdd(const std::vector<std::string> &args);
    void handleUpdate(const std::vector<std::string> &args);
    void handleDelete(const std::vector<std::string> &args);
    void handleList(const std::vector<std::string> &args);
    void handleDone(const std::vector<std::string> &args);
    void handleProgress(const std::vector<std::string> &args);

    // Enhanced command handlers
    void handleSearch(const std::vector<std::string> &args);
    void handleFilter(const std::vector<std::string> &args);
    void handleSort(const std::vector<std::string> &args);
    void handleDue(const std::vector<std::string> &args);
    void handleOverdue();
    void handleToday();

    // Utility
    void displayHelp();
    void displayTasks(const std::vector<Task> &tasks, const std::string &title = "") const;

    // Main command processor
    void processCommand(const std::vector<std::string> &args);

private:
    // Helper functions for argument parsing
    std::string findArgument(const std::vector<std::string> &args, const std::string &flag) const;
    bool hasFlag(const std::vector<std::string> &args, const std::string &flag) const;
    std::string getPrioritySymbol(const std::string &priority) const;
    std::string getStatusSymbol(const std::string &status) const;
    bool isAscendingOrder(const std::vector<std::string> &args) const;
};

#endif