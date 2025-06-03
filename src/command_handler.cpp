#include "command_handler.h"
#include "date_utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

CommandHandler::CommandHandler(TaskManager& tm) : taskManager(tm) {}

void CommandHandler::handleAdd(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a task description." << std::endl;
        std::cerr << "Usage: task_tracker add \"Task description\" [--priority high|medium|low] [--due YYYY-MM-DD]" << std::endl;
        return;
    }
    
    std::string description = args[1];
    std::string priority = findArgument(args, "--priority");
    std::string due_date = findArgument(args, "--due");
    
    // Handle short flags
    if (priority.empty()) {
        priority = findArgument(args, "-p");
    }
    if (due_date.empty()) {
        due_date = findArgument(args, "-d");
    }
    
    // Default priority to medium if not specified
    if (priority.empty()) {
        priority = "medium";
    }
    
    taskManager.addTask(description, priority, due_date);
}

void CommandHandler::handleUpdate(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: task_tracker update <id> [--description \"New desc\"] [--status pending|in_progress|done] [--priority high|medium|low] [--due YYYY-MM-DD]" << std::endl;
        return;
    }
    
    try {
        int id = std::stoi(args[1]);
        
        std::string description = findArgument(args, "--description");
        std::string status = findArgument(args, "--status");
        std::string priority = findArgument(args, "--priority");
        std::string due_date = findArgument(args, "--due");
        
        // Handle short flags
        if (priority.empty()) {
            priority = findArgument(args, "-p");
        }
        if (due_date.empty()) {
            due_date = findArgument(args, "-d");
        }
        
        // Legacy support: if only status is provided as third argument
        if (args.size() >= 3 && description.empty() && status.empty() && 
            priority.empty() && due_date.empty()) {
            status = args[2];
        }
        
        taskManager.updateTask(id, description, status, priority, due_date);
        
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
    
    // Check for filters
    std::string status_filter = findArgument(args, "--status");
    std::string priority_filter = findArgument(args, "--priority");
    
    // Legacy support: status as second argument
    if (args.size() > 1 && status_filter.empty() && priority_filter.empty()) {
        std::string filter = args[1];
        if (filter == "pending" || filter == "in_progress" || filter == "done") {
            status_filter = filter;
        }
    }
    
    // Apply filters
    tasksToDisplay = taskManager.getAllTasks();
    
    if (!status_filter.empty()) {
        tasksToDisplay = taskManager.getTasksByStatus(status_filter);
        title = status_filter + " Tasks";
    }
    
    if (!priority_filter.empty()) {
        std::vector<Task> priorityFiltered;
        for (const Task& task : tasksToDisplay) {
            if (task.getPriority() == priority_filter) {
                priorityFiltered.push_back(task);
            }
        }
        tasksToDisplay = priorityFiltered;
        title = title + " (Priority: " + priority_filter + ")";
    }
    
    // Check for special filters
    if (hasFlag(args, "--due-today")) {
        tasksToDisplay = taskManager.getTasksDueToday();
        title = "Tasks Due Today";
    } else if (hasFlag(args, "--overdue")) {
        tasksToDisplay = taskManager.getOverdueTasks();
        title = "Overdue Tasks";
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
        taskManager.updateTask(id, "", "done", "", "");
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
        taskManager.updateTask(id, "", "in_progress", "", "");
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleSearch(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a search keyword." << std::endl;
        std::cerr << "Usage: task_tracker search \"keyword\" [--status pending|in_progress|done] [--priority high|medium|low]" << std::endl;
        return;
    }
    
    std::string keyword = args[1];
    std::vector<Task> results = taskManager.searchTasksByKeyword(keyword);
    
    // Apply additional filters if specified
    std::string status_filter = findArgument(args, "--status");
    std::string priority_filter = findArgument(args, "--priority");
    
    if (!status_filter.empty()) {
        std::vector<Task> statusFiltered;
        for (const Task& task : results) {
            if (task.getStatus() == status_filter) {
                statusFiltered.push_back(task);
            }
        }
        results = statusFiltered;
    }
    
    if (!priority_filter.empty()) {
        std::vector<Task> priorityFiltered;
        for (const Task& task : results) {
            if (task.getPriority() == priority_filter) {
                priorityFiltered.push_back(task);
            }
        }
        results = priorityFiltered;
    }
    
    std::string title = "Search Results for: \"" + keyword + "\"";
    displayTasks(results, title);
}

void CommandHandler::handleFilter(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cerr << "Error: Please provide filter criteria." << std::endl;
        std::cerr << "Usage: task_tracker filter priority high|medium|low" << std::endl;
        std::cerr << "       task_tracker filter status pending|in_progress|done" << std::endl;
        return;
    }
    
    std::string filterType = args[1];
    std::string filterValue = args[2];
    
    std::vector<Task> results;
    std::string title;
    
    if (filterType == "priority") {
        results = taskManager.getTasksByPriority(filterValue);
        title = "Tasks with Priority: " + filterValue;
    } else if (filterType == "status") {
        results = taskManager.getTasksByStatus(filterValue);
        title = "Tasks with Status: " + filterValue;
    } else {
        std::cerr << "Error: Invalid filter type. Use 'priority' or 'status'." << std::endl;
        return;
    }
    
    displayTasks(results, title);
}

void CommandHandler::handleDue(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Error: Please provide a date or 'today'." << std::endl;
        std::cerr << "Usage: task_tracker due YYYY-MM-DD" << std::endl;
        std::cerr << "       task_tracker due today" << std::endl;
        return;
    }
    
    std::string dateArg = args[1];
    std::vector<Task> results;
    std::string title;
    
    if (dateArg == "today") {
        results = taskManager.getTasksDueToday();
        title = "Tasks Due Today";
    } else {
        if (!DateUtils::isValidDate(dateArg)) {
            std::cerr << "Error: Invalid date format. Please use YYYY-MM-DD." << std::endl;
            return;
        }
        results = taskManager.getTasksDueBy(dateArg);
        title = "Tasks Due By: " + dateArg;
    }
    
    displayTasks(results, title);
}

void CommandHandler::handleOverdue() {
    std::vector<Task> results = taskManager.getOverdueTasks();
    displayTasks(results, "Overdue Tasks");
}

void CommandHandler::handleToday() {
    std::vector<Task> results = taskManager.getTasksDueToday();
    displayTasks(results, "Tasks Due Today");
}

void CommandHandler::displayHelp() {
    std::cout << "Task Tracker - Command Line Task Management Tool (Phase 2)\n" << std::endl;
    std::cout << "USAGE:" << std::endl;
    std::cout << "  task_tracker <command> [arguments]\n" << std::endl;
    std::cout << "BASIC COMMANDS:" << std::endl;
    std::cout << "  add \"description\" [options]    Add a new task" << std::endl;
    std::cout << "  list [filters]                  List tasks" << std::endl;
    std::cout << "  update <id> [options]           Update task" << std::endl;
    std::cout << "  delete <id>                     Delete a task" << std::endl;
    std::cout << "  done <id>                       Mark task as done" << std::endl;
    std::cout << "  progress <id>                   Mark task as in progress\n" << std::endl;
    
    std::cout << "ENHANCED COMMANDS:" << std::endl;
    std::cout << "  search \"keyword\" [filters]      Search tasks by keyword" << std::endl;
    std::cout << "  filter <type> <value>           Filter tasks by criteria" << std::endl;
    std::cout << "  due <date|today>                Show tasks due by date" << std::endl;
    std::cout << "  overdue                         Show overdue tasks" << std::endl;
    std::cout << "  today                           Show tasks due today\n" << std::endl;
    
    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  --priority, -p <high|medium|low>  Set task priority" << std::endl;
    std::cout << "  --due, -d <YYYY-MM-DD>            Set due date" << std::endl;
    std::cout << "  --status <pending|in_progress|done>  Filter by status" << std::endl;
    std::cout << "  --description \"new description\"   Update description\n" << std::endl;
    
    std::cout << "EXAMPLES:" << std::endl;
    std::cout << "  task_tracker add \"Buy groceries\" --priority high --due 2025-06-10" << std::endl;
    std::cout << "  task_tracker list --priority high" << std::endl;
    std::cout << "  task_tracker search \"meeting\" --status pending" << std::endl;
    std::cout << "  task_tracker update 1 --priority medium --due 2025-06-15" << std::endl;
    std::cout << "  task_tracker filter priority high" << std::endl;
    std::cout << "  task_tracker due today" << std::endl;
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
    std::cout << std::left 
              << std::setw(4) << "ID" 
              << std::setw(3) << " P "
              << std::setw(25) << "Description" 
              << std::setw(15) << "Status"
              << std::setw(12) << "Due Date"
              << std::setw(10) << "Days Left" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    // Table rows
    for (const Task& task : tasks) {
        std::string statusDisplay = getStatusSymbol(task.getStatus()) + " " + task.getStatus();
        std::string prioritySymbol = getPrioritySymbol(task.getPriority());
        std::string dueDisplay = task.getDueDate().empty() ? "-" : task.getDueDate();
        
        std::string daysLeft = "-";
        if (!task.getDueDate().empty()) {
            if (task.isOverdue()) {
                daysLeft = "OVERDUE";
            } else if (task.isDueToday()) {
                daysLeft = "TODAY";
            } else {
                int days = task.getDaysUntilDue();
                if (days >= 0) {
                    daysLeft = std::to_string(days) + " days";
                }
            }
        }
        
        std::cout << std::left 
                  << std::setw(4) << task.getId()
                  << std::setw(3) << prioritySymbol
                  << std::setw(25) << (task.getDescription().length() > 24 ? 
                                      task.getDescription().substr(0, 21) + "..." : 
                                      task.getDescription())
                  << std::setw(15) << statusDisplay
                  << std::setw(12) << dueDisplay
                  << std::setw(10) << daysLeft << std::endl;
    }
    
    std::cout << "\nLegend: [!] High Priority, [>] In Progress, [X] Done, [ ] Pending" << std::endl;
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
    } else if (command == "search") {
        handleSearch(args);
    } else if (command == "filter") {
        handleFilter(args);
    } else if (command == "due") {
        handleDue(args);
    } else if (command == "overdue") {
        handleOverdue();
    } else if (command == "today") {
        handleToday();
    } else if (command == "help" || command == "--help" || command == "-h") {
        displayHelp();
    } else {
        std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
        std::cerr << "Run 'task_tracker help' for usage information." << std::endl;
    }
}

// Helper functions
std::string CommandHandler::findArgument(const std::vector<std::string>& args, const std::string& flag) const {
    auto it = std::find(args.begin(), args.end(), flag);
    if (it != args.end() && ++it != args.end()) {
        return *it;
    }
    return "";
}

bool CommandHandler::hasFlag(const std::vector<std::string>& args, const std::string& flag) const {
    return std::find(args.begin(), args.end(), flag) != args.end();
}

std::string CommandHandler::getPrioritySymbol(const std::string& priority) const {
    if (priority == "high") return "[!]";
    if (priority == "medium") return "[>]";
    if (priority == "low") return "[-]";
    return "[ ]";
}

std::string CommandHandler::getStatusSymbol(const std::string& status) const {
    if (status == "done") return "[X]";
    if (status == "in_progress") return "[>]";
    return "[ ]";
}