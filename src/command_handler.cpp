#include "command_handler.h"
#include "date_utils.h"
#include "filter_utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

CommandHandler::CommandHandler(TaskManager &tm) : taskManager(tm) {}

void CommandHandler::handleAdd(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide a task description." << std::endl;
        std::cerr << "Usage: add \"Task description\" [--priority high|medium|low] [--due YYYY-MM-DD]" << std::endl;
        return;
    }

    std::string description = args[1];
    std::string priority = findArgument(args, "--priority");
    std::string due_date = findArgument(args, "--due");

    // Handle short flags
    if (priority.empty())
    {
        priority = findArgument(args, "-p");
    }
    if (due_date.empty())
    {
        due_date = findArgument(args, "-d");
    }

    // Default priority to medium if not specified
    if (priority.empty())
    {
        priority = "medium";
    }

    taskManager.addTask(description, priority, due_date);
}

void CommandHandler::handleUpdate(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: update <id> [--description \"New desc\"] [--status pending|in_progress|done] [--priority high|medium|low] [--due YYYY-MM-DD]" << std::endl;
        return;
    }

    try
    {
        int id = std::stoi(args[1]);

        std::string description = findArgument(args, "--description");
        std::string status = findArgument(args, "--status");
        std::string priority = findArgument(args, "--priority");
        std::string due_date = findArgument(args, "--due");

        // Handle short flags
        if (priority.empty())
        {
            priority = findArgument(args, "-p");
        }
        if (due_date.empty())
        {
            due_date = findArgument(args, "-d");
        }

        // Legacy support: if only status is provided as third argument
        if (args.size() >= 3 && description.empty() && status.empty() &&
            priority.empty() && due_date.empty())
        {
            status = args[2];
        }

        taskManager.updateTask(id, description, status, priority, due_date);
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleDelete(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: delete <id>" << std::endl;
        return;
    }

    try
    {
        int id = std::stoi(args[1]);
        taskManager.deleteTask(id);
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleList(const std::vector<std::string> &args)
{
    std::string status_filter = findArgument(args, "--status");
    std::string priority_filter = findArgument(args, "--priority");
    std::string sort_by = findArgument(args, "--sort");
    bool ascending = isAscendingOrder(args);

    // Legacy support: status as second argument
    if (args.size() > 1 && status_filter.empty() && priority_filter.empty() && sort_by.empty())
    {
        std::string filter = args[1];
        if (filter == "pending" || filter == "in_progress" || filter == "done")
        {
            status_filter = filter;
        }
    }

    // Check for special filters
    bool due_today = hasFlag(args, "--due-today");
    bool overdue = hasFlag(args, "--overdue");

    // Get filtered and sorted tasks
    std::vector<Task> tasksToDisplay = taskManager.getTasksFilteredAndSorted(
        sort_by, ascending, "", priority_filter, status_filter, "", overdue, due_today);

    // Generate title
    std::string title = "All Tasks";
    if (!status_filter.empty())
    {
        title = status_filter + " Tasks";
    }
    if (!priority_filter.empty())
    {
        title += " (Priority: " + priority_filter + ")";
    }
    if (due_today)
    {
        title = "Tasks Due Today";
    }
    else if (overdue)
    {
        title = "Overdue Tasks";
    }
    if (!sort_by.empty())
    {
        title += " (Sorted by " + sort_by + (ascending ? " asc" : " desc") + ")";
    }

    displayTasks(tasksToDisplay, title);
}

void CommandHandler::handleDone(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: done <id>" << std::endl;
        return;
    }

    try
    {
        int id = std::stoi(args[1]);
        taskManager.updateTask(id, "", "done", "", "");
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleProgress(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide a task ID." << std::endl;
        std::cerr << "Usage: progress <id>" << std::endl;
        return;
    }

    try
    {
        int id = std::stoi(args[1]);
        taskManager.updateTask(id, "", "in_progress", "", "");
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Error: Invalid task ID. Please provide a valid number." << std::endl;
    }
}

void CommandHandler::handleSearch(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide a search keyword." << std::endl;
        std::cerr << "Usage: search \"keyword\" [--status pending|in_progress|done] [--priority high|medium|low] [--sort field]" << std::endl;
        return;
    }

    std::string keyword = args[1];
    std::string status_filter = findArgument(args, "--status");
    std::string priority_filter = findArgument(args, "--priority");
    std::string sort_by = findArgument(args, "--sort");
    bool ascending = isAscendingOrder(args);

    std::vector<Task> results = taskManager.getTasksFilteredAndSorted(
        sort_by, ascending, keyword, priority_filter, status_filter);

    std::string title = "Search Results for: \"" + keyword + "\"";
    if (!status_filter.empty() || !priority_filter.empty())
    {
        title += " (Filtered)";
    }

    displayTasks(results, title);
}

void CommandHandler::handleFilter(const std::vector<std::string> &args)
{
    if (args.size() < 3)
    {
        std::cerr << "Error: Please provide filter criteria." << std::endl;
        std::cerr << "Usage: filter priority high|medium|low" << std::endl;
        std::cerr << "       filter status pending|in_progress|done" << std::endl;
        return;
    }

    std::string filterType = args[1];
    std::string filterValue = args[2];

    std::vector<Task> results;
    std::string title;

    if (filterType == "priority")
    {
        results = taskManager.getTasksByPriority(filterValue);
        title = "Tasks with Priority: " + filterValue;
    }
    else if (filterType == "status")
    {
        results = taskManager.getTasksByStatus(filterValue);
        title = "Tasks with Status: " + filterValue;
    }
    else
    {
        std::cerr << "Error: Invalid filter type. Use 'priority' or 'status'." << std::endl;
        return;
    }

    displayTasks(results, title);
}

void CommandHandler::handleSort(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide sort criteria." << std::endl;
        std::cerr << "Usage: sort priority|due_date|status|id|created_date [asc|desc]" << std::endl;
        return;
    }

    std::string sort_by = args[1];
    bool ascending = true;

    if (args.size() > 2)
    {
        std::string order = args[2];
        if (order == "desc" || order == "descending")
        {
            ascending = false;
        }
    }

    if (!FilterUtils::isValidSortField(sort_by))
    {
        std::cerr << "Error: Invalid sort field. Valid fields: priority, due_date, status, id, created_date" << std::endl;
        return;
    }

    std::vector<Task> results = taskManager.getTasksSorted(sort_by, ascending);
    std::string title = "All Tasks (Sorted by " + sort_by + (ascending ? " ascending" : " descending") + ")";

    displayTasks(results, title);
}

void CommandHandler::handleDue(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cerr << "Error: Please provide a date or 'today'." << std::endl;
        std::cerr << "Usage: due YYYY-MM-DD" << std::endl;
        std::cerr << "       due today" << std::endl;
        return;
    }

    std::string dateArg = args[1];
    std::vector<Task> results;
    std::string title;

    if (dateArg == "today")
    {
        results = taskManager.getTasksDueToday();
        title = "Tasks Due Today";
    }
    else
    {
        if (!DateUtils::isValidDate(dateArg))
        {
            std::cerr << "Error: Invalid date format. Please use YYYY-MM-DD." << std::endl;
            return;
        }
        results = taskManager.getTasksDueBy(dateArg);
        title = "Tasks Due By: " + dateArg;
    }

    displayTasks(results, title);
}

void CommandHandler::handleOverdue()
{
    std::vector<Task> results = taskManager.getOverdueTasks();
    displayTasks(results, "Overdue Tasks");
}

void CommandHandler::handleToday()
{
    std::vector<Task> results = taskManager.getTasksDueToday();
    displayTasks(results, "Tasks Due Today");
}

void CommandHandler::displayHelp()
{
    std::cout << "Task Tracker - Command Line Task Management Tool (Phase 3)\n"
              << std::endl;
    std::cout << "USAGE:" << std::endl;
    std::cout << "  task_tracker <command> [arguments]\n"
              << std::endl;
    std::cout << "BASIC COMMANDS:" << std::endl;
    std::cout << "  add \"description\" [options]    Add a new task" << std::endl;
    std::cout << "  list [filters]                  List tasks with filters and sorting" << std::endl;
    std::cout << "  update <id> [options]           Update task attributes" << std::endl;
    std::cout << "  delete <id>                     Delete a task" << std::endl;
    std::cout << "  done <id>                       Mark task as done" << std::endl;
    std::cout << "  progress <id>                   Mark task as in progress\n"
              << std::endl;

    std::cout << "ADVANCED COMMANDS:" << std::endl;
    std::cout << "  search \"keyword\" [filters]      Search tasks by keyword with filters" << std::endl;
    std::cout << "  filter <type> <value>           Filter tasks by specific criteria" << std::endl;
    std::cout << "  sort <field> [asc|desc]         Sort all tasks by field" << std::endl;
    std::cout << "  due <date|today>                Show tasks due by date" << std::endl;
    std::cout << "  overdue                         Show overdue tasks" << std::endl;
    std::cout << "  today                           Show tasks due today" << std::endl;
    std::cout << "  interactive, -i                 Start interactive mode\n"
              << std::endl;

    std::cout << "OPTIONS:" << std::endl;
    std::cout << "  --priority, -p <high|medium|low>  Set/filter by task priority" << std::endl;
    std::cout << "  --due, -d <YYYY-MM-DD>            Set/filter by due date" << std::endl;
    std::cout << "  --status <pending|in_progress|done>  Filter by status" << std::endl;
    std::cout << "  --sort <field> [asc|desc]         Sort results" << std::endl;
    std::cout << "  --description \"text\"              Update description" << std::endl;
    std::cout << "  --due-today                       Filter tasks due today" << std::endl;
    std::cout << "  --overdue                         Filter overdue tasks\n"
              << std::endl;

    std::cout << "SORT FIELDS:" << std::endl;
    std::cout << "  priority, due_date, status, id, created_date\n"
              << std::endl;

    std::cout << "EXAMPLES:" << std::endl;
    std::cout << "  task_tracker add \"Review code\" --priority high --due 2025-06-15" << std::endl;
    std::cout << "  task_tracker list --priority high --sort due_date" << std::endl;
    std::cout << "  task_tracker search \"meeting\" --status pending --sort priority desc" << std::endl;
    std::cout << "  task_tracker sort priority desc" << std::endl;
    std::cout << "  task_tracker list --overdue --sort due_date" << std::endl;
    std::cout << "  task_tracker interactive" << std::endl;
}

void CommandHandler::displayTasks(const std::vector<Task> &tasks, const std::string &title) const
{
    if (!title.empty())
    {
        std::cout << "\n"
                  << title << ":" << std::endl;
        std::cout << std::string(title.length() + 1, '=') << std::endl;
    }

    if (tasks.empty())
    {
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
    for (const Task &task : tasks)
    {
        std::string statusDisplay = getStatusSymbol(task.getStatus()) + " " + task.getStatus();
        std::string prioritySymbol = getPrioritySymbol(task.getPriority());
        std::string dueDisplay = task.getDueDate().empty() ? "-" : task.getDueDate();

        std::string daysLeft = "-";
        if (!task.getDueDate().empty())
        {
            if (task.isOverdue())
            {
                daysLeft = "OVERDUE";
            }
            else if (task.isDueToday())
            {
                daysLeft = "TODAY";
            }
            else
            {
                int days = task.getDaysUntilDue();
                if (days >= 0)
                {
                    daysLeft = std::to_string(days) + " days";
                }
            }
        }

        std::cout << std::left
                  << std::setw(4) << task.getId()
                  << std::setw(3) << prioritySymbol
                  << std::setw(25) << (task.getDescription().length() > 24 ? task.getDescription().substr(0, 21) + "..." : task.getDescription())
                  << std::setw(15) << statusDisplay
                  << std::setw(12) << dueDisplay
                  << std::setw(10) << daysLeft << std::endl;
    }

    std::cout << "\nLegend: [!] High Priority, [>] Medium Priority, [-] Low Priority" << std::endl;
    std::cout << "        [X] Done, [>] In Progress, [ ] Pending" << std::endl;
    std::cout << std::endl;
}

void CommandHandler::processCommand(const std::vector<std::string> &args)
{
    if (args.empty())
    {
        displayHelp();
        return;
    }

    std::string command = args[0];

    if (command == "add")
    {
        handleAdd(args);
    }
    else if (command == "list")
    {
        handleList(args);
    }
    else if (command == "update")
    {
        handleUpdate(args);
    }
    else if (command == "delete")
    {
        handleDelete(args);
    }
    else if (command == "done")
    {
        handleDone(args);
    }
    else if (command == "progress")
    {
        handleProgress(args);
    }
    else if (command == "search")
    {
        handleSearch(args);
    }
    else if (command == "filter")
    {
        handleFilter(args);
    }
    else if (command == "sort")
    {
        handleSort(args);
    }
    else if (command == "due")
    {
        handleDue(args);
    }
    else if (command == "overdue")
    {
        handleOverdue();
    }
    else if (command == "today")
    {
        handleToday();
    }
    else if (command == "help" || command == "--help" || command == "-h")
    {
        displayHelp();
    }
    else
    {
        std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
        std::cerr << "Run 'help' for usage information." << std::endl;
    }
}

// Helper functions
std::string CommandHandler::findArgument(const std::vector<std::string> &args, const std::string &flag) const
{
    auto it = std::find(args.begin(), args.end(), flag);
    if (it != args.end() && ++it != args.end())
    {
        return *it;
    }
    return "";
}

bool CommandHandler::hasFlag(const std::vector<std::string> &args, const std::string &flag) const
{
    return std::find(args.begin(), args.end(), flag) != args.end();
}

std::string CommandHandler::getPrioritySymbol(const std::string &priority) const
{
    if (priority == "high")
        return "[!]";
    if (priority == "medium")
        return "[>]";
    if (priority == "low")
        return "[-]";
    return "[ ]";
}

std::string CommandHandler::getStatusSymbol(const std::string &status) const
{
    if (status == "done")
        return "[X]";
    if (status == "in_progress")
        return "[>]";
    return "[ ]";
}

bool CommandHandler::isAscendingOrder(const std::vector<std::string> &args) const
{
    auto it = std::find(args.begin(), args.end(), "desc");
    if (it != args.end())
        return false;

    it = std::find(args.begin(), args.end(), "descending");
    if (it != args.end())
        return false;

    return true; // Default to ascending
}