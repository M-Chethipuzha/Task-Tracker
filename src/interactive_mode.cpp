#include "interactive_mode.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

InteractiveMode::InteractiveMode(TaskManager &tm)
    : taskManager(tm), commandHandler(tm), running(false), sessionCommands(0) {}

void InteractiveMode::start()
{
    running = true;
    sessionCommands = 0;

    clearScreen();
    displayWelcome();

    std::string input;
    while (running)
    {
        displayPrompt();

        if (!std::getline(std::cin, input))
        {
            // Handle EOF (Ctrl+D)
            std::cout << std::endl;
            break;
        }

        input = trim(input);
        if (input.empty())
        {
            continue;
        }

        std::vector<std::string> args = parseInput(input);
        if (!args.empty())
        {
            if (isExitCommand(args[0]))
            {
                break;
            }

            processCommand(args);
            sessionCommands++;
        }
    }

    displayGoodbye();
    stop();
}

void InteractiveMode::stop()
{
    running = false;
    autoSave();
}

bool InteractiveMode::isRunning() const
{
    return running;
}

void InteractiveMode::displayWelcome()
{
    std::cout << "\n\033[1;36m Task Tracker Interactive\033[0m \033[2mv3.0.0\033[0m\n"
              << std::endl;

    std::cout << "\033[2m" << std::string(50, '-') << "\033[0m" << std::endl;
    std::cout << "\033[3mWelcome to the future of task management\033[0m " << std::endl;
    std::cout << "\033[2mType \033[0m\033[1mhelp\033[0m\033[2m for commands | \033[0m\033[1mexit\033[0m\033[2m to quit\033[0m" << std::endl;
    std::cout << "\033[2m" << std::string(50, '-') << "\033[0m" << std::endl;

    // Show quick stats
    displayStats();
    std::cout << std::endl;
}

void InteractiveMode::displayPrompt()
{
    std::cout << "\n\033[1;35mtask\033[0m\033[1;34m-tracker\033[0m \033[2m >\033[0m ";
    std::cout.flush();
}

void InteractiveMode::displayGoodbye()
{
    std::cout << "\n\033[2m" << std::string(50, '-') << "\033[0m" << std::endl;
    std::cout << "\033[3m\033[1;36mSession Complete\033[0m" << std::endl;
    std::cout << "\033[2m" << std::string(50, '-') << "\033[0m" << std::endl;

    std::cout << "\033[2mCommands executed:\033[0m \033[1m" << sessionCommands << "\033[0m" << std::endl;
    std::cout << "\033[2mAuto-save:\033[0m \033[1;32m\033[0m \033[2mEnabled\033[0m" << std::endl;

    std::cout << "\n\033[3mThanks for using Task Tracker!\033[0m \033[2mAll changes saved.\033[0m" << std::endl;
    std::cout << "\033[1;33m Stay productive! \033[0m\n"
              << std::endl;
}

void InteractiveMode::displayStats()
{
    std::vector<Task> allTasks = taskManager.getAllTasks();
    std::vector<Task> pending = taskManager.getTasksByStatus("pending");
    std::vector<Task> inProgress = taskManager.getTasksByStatus("in_progress");
    std::vector<Task> done = taskManager.getTasksByStatus("done");
    std::vector<Task> overdue = taskManager.getOverdueTasks();
    std::vector<Task> dueToday = taskManager.getTasksDueToday();

    std::cout << "\n\033[3m\033[1;37mTask Overview\033[0m" << std::endl;
    std::cout << "\033[2mTotal:\033[0m \033[1m" << allTasks.size() << "\033[0m";
    std::cout << "\033[2m  |  Pending:\033[0m \033[1;33m" << pending.size() << "\033[0m";
    std::cout << "\033[2m  |  Active:\033[0m \033[1;34m" << inProgress.size() << "\033[0m";
    std::cout << "\033[2m  |  Done:\033[0m \033[1;32m" << done.size() << "\033[0m" << std::endl;

    if (!overdue.empty() || !dueToday.empty())
    {
        std::cout << "\033[2mAlerts:\033[0m";
        if (!overdue.empty())
        {
            std::cout << " \033[1;31m⚠\033[0m \033[2mOverdue:\033[0m \033[1;31m" << overdue.size() << "\033[0m";
        }
        if (!dueToday.empty())
        {
            std::cout << " \033[1;33m⏰\033[0m \033[2mDue today:\033[0m \033[1;33m" << dueToday.size() << "\033[0m";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> InteractiveMode::parseInput(const std::string &input)
{
    return tokenize(input);
}

void InteractiveMode::processCommand(const std::vector<std::string> &args)
{
    if (args.empty())
        return;

    try
    {
        // Handle special interactive commands first
        handleSpecialCommands(args);

        if (running)
        { // Only process if we haven't exited
            // Use the command handler for regular commands
            commandHandler.processCommand(args);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void InteractiveMode::autoSave()
{
    // The TaskManager automatically saves on each operation
    // This method could be extended for additional auto-save functionality
}

void InteractiveMode::clearScreen()
{
    // Clear screen using ANSI escape sequences (works on modern terminals)
    std::cout << "\033[2J\033[H" << std::flush;

// Fallback for older systems
#ifdef _WIN32
    // Additional Windows clearing for older CMD
    if (system("cls") != 0)
    {
        // If system call fails, use alternative method
        for (int i = 0; i < 50; ++i)
        {
            std::cout << "\n";
        }
    }
#else
    // Unix/Linux/macOS
    if (system("clear") != 0)
    {
        // If system call fails, use alternative method
        for (int i = 0; i < 50; ++i)
        {
            std::cout << "\n";
        }
    }
#endif
}

void InteractiveMode::showHelp()
{
    std::cout << "\n\033[1;36 Interactive Commands\033[0m \033[2mv3.0.0\033[0m" << std::endl;
    std::cout << "\033[2m" << std::string(60, '-') << "\033[0m" << std::endl;

    std::cout << "\n\033[3m\033[1;37mTask Management\033[0m" << std::endl;
    std::cout << "\033[1;32madd\033[0m \033[2m\"description\"\033[0m [\033[2m--priority\033[0m] [\033[2m--due\033[0m]   \033[2mCreate new task\033[0m" << std::endl;
    std::cout << "\033[1;32mlist\033[0m [\033[2m--priority\033[0m] [\033[2m--status\033[0m] [\033[2m--sort\033[0m]        \033[2mShow tasks\033[0m" << std::endl;
    std::cout << "\033[1;32mupdate\033[0m \033[2m<id>\033[0m [\033[2moptions\033[0m]                      \033[2mModify task\033[0m" << std::endl;
    std::cout << "\033[1;32mdelete\033[0m \033[2m<id>\033[0m  |  \033[1;32mdone\033[0m \033[2m<id>\033[0m  |  \033[1;32mprogress\033[0m \033[2m<id>\033[0m   \033[2mQuick actions\033[0m" << std::endl;

    std::cout << "\n\033[3m\033[1;37mAdvanced Features\033[0m" << std::endl;
    std::cout << "\033[1;34msearch\033[0m \033[2m\"keyword\"\033[0m [\033[2mfilters\033[0m]                \033[2mFind tasks\033[0m" << std::endl;
    std::cout << "\033[1;34mfilter\033[0m \033[2mpriority|status\033[0m \033[2m<value>\033[0m           \033[2mFilter by criteria\033[0m" << std::endl;
    std::cout << "\033[1;34msort\033[0m \033[2mpriority|due_date|status\033[0m [\033[2masc|desc\033[0m]   \033[2mSort tasks\033[0m" << std::endl;
    std::cout << "\033[1;34mdue\033[0m \033[2mtoday|<date>\033[0m  |  \033[1;34moverdue\033[0m  |  \033[1;34mtoday\033[0m      \033[2mDate views\033[0m" << std::endl;

    std::cout << "\n\033[3m\033[1;37mSession Controls\033[0m" << std::endl;
    std::cout << "\033[1;35mhelp\033[0m, \033[1;35mh\033[0m        \033[2mShow this help\033[0m" << std::endl;
    std::cout << "\033[1;35mstats\033[0m           \033[2mTask statistics\033[0m" << std::endl;
    std::cout << "\033[1;35mclear\033[0m, \033[1;35mcls\033[0m     \033[2mClear screen\033[0m" << std::endl;
    std::cout << "\033[1;35mexit\033[0m, \033[1;35mquit\033[0m, \033[1;35mq\033[0m  \033[2mEnd session\033[0m" << std::endl;

    std::cout << "\n\033[3m\033[2mExamples:\033[0m" << std::endl;
    std::cout << "\033[2m  add \"Review code\" -p high -d 2025-06-15\033[0m" << std::endl;
    std::cout << "\033[2m  list --sort priority --status pending\033[0m" << std::endl;
    std::cout << "\033[2m  search \"meeting\" --priority high\033[0m" << std::endl;
    std::cout << "\033[2m  sort due_date desc\033[0m" << std::endl;

    std::cout << "\033[2m" << std::string(60, '-') << "\033[0m" << std::endl;
}

std::string InteractiveMode::trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> InteractiveMode::tokenize(const std::string &input)
{
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    bool inQuotes = false;
    std::string currentToken;

    for (size_t i = 0; i < input.length(); ++i)
    {
        char c = input[i];

        if (c == '"' && (i == 0 || input[i - 1] != '\\'))
        {
            inQuotes = !inQuotes;
        }
        else if (c == ' ' && !inQuotes)
        {
            if (!currentToken.empty())
            {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        }
        else
        {
            currentToken += c;
        }
    }

    if (!currentToken.empty())
    {
        tokens.push_back(currentToken);
    }

    return tokens;
}

bool InteractiveMode::isExitCommand(const std::string &command)
{
    return (command == "exit" || command == "quit" || command == "q");
}

void InteractiveMode::handleSpecialCommands(const std::vector<std::string> &args)
{
    if (args.empty())
        return;

    std::string command = args[0];

    if (command == "help" || command == "h")
    {
        showHelp();
    }
    else if (command == "stats")
    {
        displayStats();
    }
    else if (command == "clear" || command == "cls")
    {
        clearScreen();
        displayWelcome();
    }
    else if (isExitCommand(command))
    {
        running = false;
    }
    // If not a special command, let the command handler process it
}