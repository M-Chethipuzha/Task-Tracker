# Task Tracker 

A comprehensive command-line interface (CLI) application for managing daily tasks with advanced filtering, sorting capabilities, and an interactive shell mode for seamless task management.

## 🆕 New Features 

### 🎮 **Interactive Mode**

- **Seamless Shell Experience**: Start interactive mode with `task_tracker interactive` or `task_tracker -i`
- **Auto-save**: All changes are automatically saved during the session
- **Session Statistics**: Track commands executed and view task statistics
- **Special Commands**: `help`, `stats`, `clear`, `exit` commands for enhanced UX
- **Elegant Interface**: Styled prompts, borders, and formatted output

### 🔧 **Advanced Filtering & Sorting**

- **FilterUtils Namespace**: Complete filtering and sorting algorithms
- **Combined Operations**: Filter and sort in single commands
- **Multiple Sort Fields**: Sort by priority, due_date, status, id, created_date
- **Advanced Search**: Search with multiple filters and sorting options
- **Enhanced List Command**: Supports complex filtering combinations

### 📊 **Enhanced Command Processing**

- **Flexible CLI**: Support for multiple options and complex command structures
- **Smart Argument Parsing**: Handles quoted strings and multiple parameters
- **Rich Output**: Enhanced table formatting with visual indicators
- **Comprehensive Help**: Context-aware help system

## 🚀 Building the Project

### Prerequisites

- CMake 3.16 or higher
- C++17 compliant compiler (GCC, Clang, or MSVC)

### Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure the project
cmake ..

# Build the project
cmake --build .

# Run tests (optional)
ctest --verbose
```

The executable will be created in `build/bin/task_tracker`.

## 📖 Usage

### 🎮 Interactive Mode

**Start Interactive Mode:**

```bash
./task_tracker interactive
# or
./task_tracker -i
```

**Interactive Session Example:**

```bash
╔════════════════════════════════════════════════════════════════╗
║                    Task Tracker Interactive Mode               ║
║                         Phase 3 - v3.0.0                      ║
╚════════════════════════════════════════════════════════════════╝

Welcome to Task Tracker Interactive Mode!
Type 'help' for available commands or 'exit' to quit.
All changes are automatically saved.

📊 Quick Stats:
   Total tasks: 5 | Pending: 3 | In Progress: 1 | Done: 1
   ⚠️  Overdue: 1 | Due Today: 2

task-tracker> add "Review pull request" --priority high --due 2025-06-15
Task added successfully with ID: 6 (Priority: high) (Due: 2025-06-15)

task-tracker> list --priority high --sort due_date

All Tasks (Priority: high) (Sorted by due_date asc):
==================================================
ID  P  Description              Status         Due Date    Days Left
----------------------------------------------------------------------
1   [!] Finish project report   [>] in_progress 2025-06-10  7 days
6   [!] Review pull request     [ ] pending     2025-06-15  12 days

task-tracker> search "project" --sort priority desc

Search Results for: "project":
==============================
ID  P  Description              Status         Due Date    Days Left
----------------------------------------------------------------------
1   [!] Finish project report   [>] in_progress 2025-06-10  7 days

task-tracker> stats
📊 Quick Stats:
   Total tasks: 6 | Pending: 4 | In Progress: 1 | Done: 1
   ⚠️  Overdue: 1 | Due Today: 2

task-tracker> exit

┌─────────────────────────────────────────────────────┐
│                 Session Summary                     │
├─────────────────────────────────────────────────────┤
│ Commands executed:                               4  │
│ Auto-save:                                 Enabled  │
└─────────────────────────────────────────────────────┘

Thank you for using Task Tracker! All changes have been saved.
Goodbye! 👋
```

### 🔧 Advanced Command-Line Usage

#### **Enhanced List Command**

```bash
# List with multiple filters and sorting
./task_tracker list --priority high --status pending --sort due_date

# List overdue tasks sorted by priority
./task_tracker list --overdue --sort priority desc

# List tasks due today
./task_tracker list --due-today
```

#### **Advanced Search**

```bash
# Search with filters and sorting
./task_tracker search "meeting" --priority high --sort due_date

# Search pending tasks containing "project"
./task_tracker search "project" --status pending --sort priority desc
```

#### **Comprehensive Sorting**

```bash
# Sort all tasks by different fields
./task_tracker sort priority desc
./task_tracker sort due_date asc
./task_tracker sort status
./task_tracker sort created_date desc
```

#### **Complex Filtering**

```bash
# Filter by priority
./task_tracker filter priority high

# Filter by status
./task_tracker filter status pending

# Combine with sorting in list command
./task_tracker list --priority medium --sort due_date desc
```

#### **Date-based Operations**

```bash
# View tasks due today
./task_tracker due today
./task_tracker today

# View tasks due by specific date
./task_tracker due 2025-06-15

# View all overdue tasks
./task_tracker overdue
```

## 📊 Enhanced Features

### **🎯 Filter & Sort Combinations**

All commands now support advanced filtering and sorting:

```bash
# Examples of powerful combinations
./task_tracker list --priority high --status pending --sort due_date
./task_tracker search "urgent" --priority high --sort created_date desc
./task_tracker list --overdue --sort priority desc
```

### **🔍 Advanced Search Capabilities**

- **Case-insensitive keyword matching**
- **Multiple filter criteria**
- **Integrated sorting options**
- **Real-time results with rich formatting**

### **📈 Enhanced Sorting Options**

**Available Sort Fields:**

- `priority` - Sort by task priority (high > medium > low)
- `due_date` - Sort by due date (earliest first by default)
- `status` - Sort by status (pending > in_progress > done)
- `id` - Sort by task ID
- `created_date` - Sort by creation date

**Sort Orders:**

- `asc` or `ascending` (default)
- `desc` or `descending`

## 🏗️ Architecture Overview

### **New Components in Phase 3**

#### **FilterUtils Namespace**

```cpp
namespace FilterUtils {
    // Core filtering functions
    std::vector<Task> filterByKeyword(const std::vector<Task>& tasks, const std::string& keyword);
    std::vector<Task> filterByPriority(const std::vector<Task>& tasks, const std::string& priority);
    std::vector<Task> filterByStatus(const std::vector<Task>& tasks, const std::string& status);

    // Advanced sorting functions
    std::vector<Task> sortByPriority(std::vector<Task> tasks, bool ascending = false);
    std::vector<Task> sortByDueDate(std::vector<Task> tasks, bool ascending = true);
    std::vector<Task> sortByStatus(std::vector<Task> tasks, bool ascending = true);

    // Combined operations
    std::vector<Task> filterAndSort(/* multiple parameters */);
}
```

#### **InteractiveMode Class**

```cpp
class InteractiveMode {
public:
    void start();                    // Main interactive loop
    void displayWelcome();          // Styled welcome message
    void displayStats();            // Show task statistics
    void processCommand();           // Process interactive commands

private:
    std::vector<std::string> tokenize();  // Smart input parsing
    void handleSpecialCommands();         // Interactive-only commands
};
```

#### **Enhanced TaskManager**

```cpp
class TaskManager {
public:
    // New advanced methods
    std::vector<Task> getTasksFiltered(/* filter criteria */);
    std::vector<Task> getTasksSorted(const std::string& sort_by, bool ascending);
    std::vector<Task> getTasksFilteredAndSorted(/* combined parameters */);
};
```

## 📋 Complete Command Reference

### **Basic Commands**

| Command    | Description                     | Example                                       |
| ---------- | ------------------------------- | --------------------------------------------- |
| `add`      | Add new task with options       | `add "Task" --priority high --due 2025-06-15` |
| `list`     | List tasks with filters/sorting | `list --priority high --sort due_date`        |
| `update`   | Update task attributes          | `update 1 --status done --priority medium`    |
| `delete`   | Delete task by ID               | `delete 1`                                    |
| `done`     | Mark task as done               | `done 1`                                      |
| `progress` | Mark task as in progress        | `progress 1`                                  |

### **Advanced Commands**

| Command   | Description                 | Example                                            |
| --------- | --------------------------- | -------------------------------------------------- |
| `search`  | Search with filters         | `search "meeting" --priority high --sort due_date` |
| `filter`  | Filter by specific criteria | `filter priority high`                             |
| `sort`    | Sort all tasks              | `sort due_date desc`                               |
| `due`     | Show tasks due by date      | `due today` or `due 2025-06-15`                    |
| `overdue` | Show overdue tasks          | `overdue`                                          |
| `today`   | Show tasks due today        | `today`                                            |

### **Interactive Mode Commands**

| Command             | Description                   |
| ------------------- | ----------------------------- |
| `interactive`, `-i` | Start interactive mode        |
| `help`, `h`         | Show help in interactive mode |
| `stats`             | Show task statistics          |
| `clear`, `cls`      | Clear screen                  |
| `exit`, `quit`, `q` | Exit interactive mode         |

### **Options & Flags**

| Option          | Short | Description            | Example                    |
| --------------- | ----- | ---------------------- | -------------------------- |
| `--priority`    | `-p`  | Set/filter by priority | `--priority high`          |
| `--due`         | `-d`  | Set/filter by due date | `--due 2025-06-15`         |
| `--status`      |       | Filter by status       | `--status pending`         |
| `--sort`        |       | Sort results           | `--sort priority desc`     |
| `--description` |       | Update description     | `--description "New text"` |
| `--due-today`   |       | Filter tasks due today | `--due-today`              |
| `--overdue`     |       | Filter overdue tasks   | `--overdue`                |

## 📁 Project Structure

```
src/
├── main.cpp              # Entry point with interactive mode support
├── task.h/cpp            # Task class with comparison operators
├── task_manager.h/cpp    # Enhanced task management with FilterUtils integration
├── json_parser.h/cpp     # JSON serialization (unchanged)
├── command_handler.h/cpp # Enhanced command processing with advanced options
├── date_utils.h/cpp      # Date utilities with range functions
├── filter_utils.h/cpp    # ⭐ NEW: Advanced filtering and sorting algorithms
└── interactive_mode.h/cpp # ⭐ NEW: Interactive shell implementation

tests/
├── test_task.cpp         # Task class tests
├── test_json_parser.cpp  # JSON parser tests
├── test_date_utils.cpp   # Date utilities tests
├── test_enhanced_task.cpp # Enhanced task feature tests
├── test_filter_utils.cpp # ⭐ NEW: FilterUtils tests
└── CMakeLists.txt        # Updated test configuration

CMakeLists.txt            # Updated build configuration (v3.0.0)
README.md                 # This comprehensive guide
.gitignore               # Comprehensive gitignore file
```

## 🧪 Testing

### **Run All Tests**

```bash
cd build
ctest --verbose
```

### **Individual Test Suites**

```bash
# Run specific test suites
./tests/test_filter_utils
./tests/test_enhanced_task
./tests/test_date_utils
```

### **New Test Coverage**

- **FilterUtils tests**: Filtering, sorting, and combined operations
- **Interactive mode integration**: Command parsing and session management
- **Advanced argument parsing**: Complex command structures
- **Enhanced task operations**: Priority sorting and date filtering

## 🎯 Performance Optimizations

### **Efficient Algorithms**

- **In-place sorting**: Minimal memory allocation for large task lists
- **Lazy evaluation**: Filters applied in optimal order
- **Smart caching**: TaskManager maintains optimized data structures

### **Memory Management**

- **Copy-efficient operations**: Move semantics where appropriate
- **Minimal string copying**: Reference-based operations
- **Optimized JSON parsing**: Stream-based processing for large files

## 📈 Usage Statistics

Task Tracker now tracks and displays helpful statistics:

```bash
📊 Quick Stats:
   Total tasks: 15 | Pending: 8 | In Progress: 4 | Done: 3
   ⚠️  Overdue: 2 | Due Today: 1
```

**Metrics include:**

- Total task count by status
- Overdue task warnings
- Due today notifications
- Session command tracking (in interactive mode)

## 🔮 Future Enhancements

**Potential Phase 4 features:**

- **Recurring tasks**: Schedule repeating tasks
- **Task dependencies**: Link related tasks
- **Time tracking**: Log time spent on tasks
- **Export/import**: CSV and other format support
- **Collaboration**: Multi-user task sharing
- **Notifications**: System notifications for due tasks
- **Templates**: Pre-defined task templates
- **Themes**: Customizable output styling

## 🏆 Phase 3 Achievements

✅ **Complete FilterUtils implementation** with 10+ filtering and sorting functions  
✅ **Full interactive mode** with session management and auto-save  
✅ **Advanced CLI** supporting complex option combinations  
✅ **Enhanced user experience** with styled output and helpful statistics  
✅ **Comprehensive testing** with 5 test suites covering all components  
✅ **Performance optimized** algorithms for filtering and sorting  
✅ **Backward compatible** with all Phase 1 and Phase 2 functionality

## 📝 Examples Gallery

### **Power User Workflows**

**Daily Task Review:**

```bash
# Start interactive mode for quick daily management
./task_tracker interactive

> stats
> list --overdue --sort priority desc
> list --due-today
> add "Review daily standup notes" -p medium -d 2025-06-04
> exit
```

**Project Management:**

```bash
# Complex project task filtering
./task_tracker search "project alpha" --priority high --sort due_date
./task_tracker list --status in_progress --sort priority desc
./task_tracker filter priority high | grep "alpha"
```

**Weekly Planning:**

```bash
# Planning next week's tasks
./task_tracker sort due_date asc
./task_tracker list --priority high --sort due_date
./task_tracker due 2025-06-15  # Check specific deadline
```

## 📄 License

This project is for educational purposes and demonstrates advanced C++ CLI application development with modern design patterns and user experience considerations.

---
