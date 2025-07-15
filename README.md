# Task Tracker 📝

A powerful command-line task management application built with C++. Perfect for developers who want to manage their daily tasks efficiently from the terminal!

## What is Task Tracker?

Task Tracker is a CLI (Command Line Interface) application that helps you:
- ✅ Add and manage your daily tasks
- 🎯 Set priorities (high, medium, low)
- 📅 Set due dates and track deadlines
- 🔍 Search and filter tasks
- 📊 View task statistics
- 🎮 Use interactive mode for easier task management

## Quick Start

### 1. Build the Project

First, make sure you have:
- CMake 3.16 or higher
- A C++17 compiler (like GCC or Clang)

```bash
# Clone or download the project, then:
mkdir build && cd build
cmake ..
cmake --build .
```

Your executable will be in `build/bin/task_tracker`.

### 2. Your First Task

```bash
# Add your first task
./task_tracker add "Learn C++ basics"

# List all tasks
./task_tracker list

# Mark it as done
./task_tracker done 1
```

That's it! You're now using Task Tracker! 🎉

## Two Ways to Use Task Tracker

### Option 1: Interactive Mode (Recommended for Beginners)

Interactive mode is like having a conversation with your task manager. It's easier and more user-friendly:

```bash
# Start interactive mode
./task_tracker interactive
```

You'll see something like this:
```
╔════════════════════════════════════════════════════════════════╗
║                    Task Tracker Interactive Mode               ║
║                         Phase 3 - v3.0.0                       ║
╚════════════════════════════════════════════════════════════════╝

Welcome to Task Tracker Interactive Mode!
Type 'help' for available commands or 'exit' to quit.

task-tracker> 
```

Now you can type commands without the `./task_tracker` prefix:

```bash
# Add a task with priority and due date
task-tracker> add "Review code changes" --priority high --due 2025-06-15

# See all your tasks
task-tracker> list

# Check your statistics
task-tracker> stats

# Exit when done
task-tracker> exit
```

### Option 2: Single Commands

You can also run individual commands:

```bash
# Add a task
./task_tracker add "Buy groceries" --priority medium --due 2025-06-10

# List all tasks
./task_tracker list

# Update a task
./task_tracker update 1 --status done
```

## Essential Commands (Start Here!)

### Basic Task Management

```bash
# Add a simple task
add "Write documentation"

# Add a task with priority and due date
add "Review pull request" --priority high --due 2025-06-15

# List all tasks
list

# Mark task as done (replace 1 with your task ID)
done 1

# Delete a task
delete 1

# Update a task
update 1 --priority high --description "Updated task description"
```

### Viewing Your Tasks

```bash
# See all tasks
list

# See only high priority tasks
list --priority high

# See tasks that are overdue
list --overdue

# See tasks due today
list --due-today

# Search for specific tasks
search "meeting"
```

### Getting Help

```bash
# In interactive mode
help

# See your task statistics
stats
```

## Understanding Task Properties

Each task has these properties:

| Property | Description | Example Values |
|----------|-------------|----------------|
| **ID** | Unique number for each task | 1, 2, 3... |
| **Description** | What you need to do | "Write README file" |
| **Priority** | How important it is | high, medium, low |
| **Status** | Current state | pending, in_progress, done |
| **Due Date** | When it's due | 2025-06-15 |
| **Created Date** | When you added it | Auto-generated |

## Common Workflows

### Daily Task Review
```bash
# Start interactive mode
./task_tracker interactive

# Check what's urgent
task-tracker> list --overdue
task-tracker> list --due-today

# Add today's tasks
task-tracker> add "Daily standup" --priority medium --due 2025-06-04

# Check your progress
task-tracker> stats

# Exit
task-tracker> exit
```

### Weekly Planning
```bash
# See all tasks sorted by due date
./task_tracker sort due_date

# Focus on high priority items
./task_tracker list --priority high --sort due_date

# Check specific deadline
./task_tracker due 2025-06-15
```

## Advanced Features (For Later)

Once you're comfortable with the basics, you can explore these powerful features:

### Filtering and Sorting
```bash
# Show high priority pending tasks, sorted by due date
list --priority high --status pending --sort due_date

# Show overdue tasks sorted by priority
list --overdue --sort priority desc

# Search with filters
search "project" --priority high --sort due_date
```

### Different Sort Options
```bash
# Sort by priority (high to low)
sort priority desc

# Sort by due date (earliest first)
sort due_date asc

# Sort by when you created them
sort created_date desc
```

## How Tasks Are Displayed

When you run `list`, you'll see something like:
```
All Tasks:
==========
ID  P  Description              Status         Due Date    Days Left
----------------------------------------------------------------------
1   [!] Finish project report   [>] in_progress 2025-06-10  7 days
2   [!] Review pull request     [ ] pending     2025-06-15  12 days
3   [-] Buy groceries           [✓] done        2025-06-08  -
```

**Legend:**
- `[!]` = High priority
- `[~]` = Medium priority  
- `[-]` = Low priority
- `[ ]` = Pending
- `[>]` = In progress
- `[✓]` = Done

## All Available Commands

### Basic Commands
| Command | What it does | Example |
|---------|--------------|---------|
| `add` | Create a new task | `add "Buy milk" --priority high` |
| `list` | Show your tasks | `list` or `list --priority high` |
| `update` | Change a task | `update 1 --status done` |
| `delete` | Remove a task | `delete 1` |
| `done` | Mark as completed | `done 1` |
| `progress` | Mark as in progress | `progress 1` |

### Advanced Commands
| Command | What it does | Example |
|---------|--------------|---------|
| `search` | Find tasks | `search "meeting" --priority high` |
| `filter` | Show filtered tasks | `filter priority high` |
| `sort` | Sort all tasks | `sort due_date desc` |
| `due` | Tasks due on date | `due today` or `due 2025-06-15` |
| `overdue` | Show overdue tasks | `overdue` |
| `today` | Show today's tasks | `today` |

### Interactive Mode Only
| Command | What it does |
|---------|--------------|
| `help` | Show help |
| `stats` | Show statistics |
| `clear` | Clear screen |
| `exit` | Quit interactive mode |

## Options You Can Use

| Option | Short | What it does | Example |
|--------|-------|--------------|---------|
| `--priority` | `-p` | Set or filter by priority | `--priority high` |
| `--due` | `-d` | Set or filter by due date | `--due 2025-06-15` |
| `--status` | | Filter by status | `--status pending` |
| `--sort` | | Sort results | `--sort priority desc` |
| `--description` | | Update description | `--description "New text"` |
| `--due-today` | | Show tasks due today | `--due-today` |
| `--overdue` | | Show overdue tasks | `--overdue` |

## Project Structure (For Developers)

```
src/
├── main.cpp              # Program entry point
├── task.h/cpp            # Task class definition
├── task_manager.h/cpp    # Task management logic
├── json_parser.h/cpp     # Save/load tasks
├── command_handler.h/cpp # Process commands
├── date_utils.h/cpp      # Date handling
├── filter_utils.h/cpp    # Filtering and sorting
└── interactive_mode.h/cpp # Interactive shell

tests/
├── test_*.cpp            # Unit tests
└── CMakeLists.txt        # Test configuration

CMakeLists.txt            # Build configuration
README.md                 # This file
.gitignore               # Git ignore rules
```

## Testing

To run tests and make sure everything works:

```bash
cd build
ctest --verbose
```

## Tips for Beginners

1. **Start Simple**: Begin with just `add` and `list` commands
2. **Use Interactive Mode**: It's more beginner-friendly than single commands
3. **Check `stats`**: Great way to see your progress
4. **Don't Worry About Advanced Features**: Master the basics first
5. **Practice Daily**: Use it for your actual tasks to learn faster

## Getting Help

- Type `help` in interactive mode
- All commands show usage if you use them incorrectly
- Check the examples in this README
- Look at the test files for more usage examples

## What's Special About This Version

This version includes:
- 🎮 **Interactive Mode**: Easier to use, like chatting with your task manager
- 🔧 **Advanced Filtering**: Find exactly the tasks you need
- 📊 **Better Statistics**: See your productivity at a glance
- 🎯 **Smart Sorting**: Organize tasks however you want
- 🚀 **Better Performance**: Faster with large numbers of tasks

## Next Steps

Once you're comfortable with Task Tracker:

1. Try the advanced filtering options
2. Use it for a real project
3. Explore the source code to learn C++
4. Contribute improvements
5. Build your own CLI applications!

## Future Updates & Roadmap 🚀

We're planning to add these exciting features in upcoming versions:

### Planned Features
- **🔔 Reminders**: Get notified when tasks are due
- **📋 Task Templates**: Save common task patterns (e.g., "Daily Standup", "Code Review")
- **⏰ Time Tracking**: Log how long you spend on tasks
- **🏷️ Tags**: Add custom tags to organize tasks better
- **🔄 Recurring Tasks**: Set up daily/weekly/monthly repeating tasks

---

**Happy task tracking!** 🎉

*This project demonstrates modern C++ development practices and CLI application design. Perfect for learning C++ while building something useful!*

