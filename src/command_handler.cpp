#include "command_handler.h"
#include <iostream>
#include <iomanip>

CommandHandler::CommandHandler(TaskManager& tm) : taskManager(tm) {}

void CommandHandler::handleAdd(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a task description." << std::endl;
        std::cerr << "Usage: task_tracker add \"Task description\"" << std::endl;
        return;
    }
    
    std::string description = args[1];
    taskManager.addTask(description);
}

void CommandHandler::handleUpdate(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cerr << "Error: Please provide task ID and new status." << std::endl;
        std::cerr << "Usage: task_tracker update <id> <status>" << std::endl;
        std::cerr << "Valid statuses: pending, in_progress, done" << std::endl;
        return;
    }
    
    try {
        int id = std::stoi(args[1]);
        std::string status = args[2];
        taskManager.updateTask(id, status);
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleDelete(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: task_tracker delete <id>" << std::endl;
        return;
    }
    
    try {
        int id = std::stoi(args[1]);
        taskManager.deleteTask(id);
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleList(const std::vector<std::string>& args) {
    std::vector<Task> tasksToDisplay;
    std::string title = "All Tasks";
    
    if (args.size() > 1) {
        std::string filter = args[1];
        if (filter == "pending" || filter == "in_progress" || filter == "done") {
            tasksToDisplay = taskManager.getTasksByStatus(filter);
            title = filter + " Tasks";
        } else {
            std::cerr << "Error: Invalid status filter. Valid options: pending, in_progress, done" << std::endl;
            return;
        }
    } else {
        tasksToDisplay = taskManager.getAllTasks();
    }
    
    displayTasks(tasksToDisplay, title);
}

void CommandHandler::handleDone(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: task_tracker done <id>" << std::endl;
        return;
    }
    
    try {
        int id = std::stoi(args[1]);
        taskManager.updateTask(id, "done");
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleProgress(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: task_tracker progress <id>" << std::endl;
        return;
    }
    
    try {
        int id = std::stoi(args[1]);
        taskManager.updateTask(id, "in_progress");
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::displayHelp() {
    std::cout << "Task Tracker - Command Line Task Management Tool\n" << std::endl;
    std::cout << "USAGE:" << std::endl;
    std::cout << "  task_tracker <command> [arguments]\n" << std::endl;
    std::cout << "COMMANDS:" << std::endl;
    std::cout << "  add \"description\"     Add a new task" << std::endl;
    std::cout << "  list [status]          List all tasks or filter by status" << std::endl;
    std::cout << "  update <id> <status>   Update task status" << std::endl;
    std::cout << "  delete <id>            Delete a task" << std::endl;
    std::cout << "  done <id>              Mark task as done" << std::endl;
    std::cout << "  progress <id>          Mark task as in progress" << std::endl;
    std::cout << "  help                   Show this help message\n" << std::endl;
    std::cout << "EXAMPLES:" << std::endl;
    std::cout << "  task_tracker add \"Buy groceries\"" << std::endl;
    std::cout << "  task_tracker list" << std::endl;
    std::cout << "  task_tracker list pending" << std::endl;
    std::cout << "  task_tracker update 1 in_progress" << std::endl;
    std::cout << "  task_tracker done 1" << std::endl;
    std::cout << "  task_tracker delete 1" << std::endl;
}

void CommandHandler::displayTasks(const std::vector<Task>& tasks, const std::string& title) const {
    if (!title.empty()) {
        std::cout << "\n" << title << ":" << std::endl;
        std::cout << std::string(title.length() + 1, '=') << std::endl;
    }
    
    if (tasks.empty()) {
        std::cout << "No tasks found." << std::endl;
        return;
    }
    
    // Table header
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(30) << "Description" 
              << std::setw(15) << "Status" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    // Table rows
    for (const Task& task : tasks) {
        std::string statusDisplay = task.getStatus();
        
        // Add visual indicators
        if (task.getStatus() == "done") {
            statusDisplay = "[X] " + statusDisplay;
        } else if (task.getStatus() == "in_progress") {
            statusDisplay = "[>] " + statusDisplay;
        } else {
            statusDisplay = "[ ] " + statusDisplay;
        }
        
        std::cout << std::left << std::setw(5) << task.getId()
                  << std::setw(30) << task.getDescription()
                  << std::setw(15) << statusDisplay << std::endl;
    }
    std::cout << std::endl;
}

void CommandHandler::processCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        displayHelp();
        return;
    }
    
    std::string command = args[0];
    
    if (command == "add") {
        handleAdd(args);
    } else if (command == "list") {
        handleList(args);
    } else if (command == "update") {
        handleUpdate(args);
    } else if (command == "delete") {
        handleDelete(args);
    } else if (command == "done") {
        handleDone(args);
    } else if (command == "progress") {
        handleProgress(args);
    } else if (command == "help" || command == "--help" || command == "-h") {
        displayHelp();
    } else {
        std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
        std::cerr << "Run 'task_tracker help' for usage information." << std::endl;
    }
}