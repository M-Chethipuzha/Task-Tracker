# Task Tracker - Phase 2 (Enhanced Attributes & Filtering)

A command-line interface (CLI) application for managing daily tasks with enhanced features including priority levels, due dates, and advanced filtering capabilities.

## New Features in Phase 2

- **Priority Levels**: Set task priorities as high, medium, or low
- **Due Dates**: Assign due dates to tasks with automatic overdue detection
- **Enhanced Filtering**: Filter tasks by priority, status, due dates, and keywords
- **Advanced Search**: Search tasks by keywords with additional filters
- **Date-based Views**: View tasks due today, overdue tasks, or tasks due by specific dates
- **Rich Display**: Enhanced table output with priority indicators and due date information

## Building the Project

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
ctest
```

The executable will be created in `build/bin/task_tracker`.

## Usage

### Enhanced Commands

#### Adding Tasks with Priority and Due Dates
```bash
# Add task with high priority and due date
./task_tracker add "Finish project report" --priority high --due 2025-06-15

# Add task with short flags
./task_tracker add "Buy groceries" -p medium -d 2025-06-10

# Add task with just priority
./task_tracker add "Call client" --priority high
```

#### Advanced Listing and Filtering
```bash
# List tasks by priority
./task_tracker list --priority high
./task_tracker list --priority medium

# List tasks by status (enhanced)
./task_tracker list --status pending
./task_tracker list --priority high --status in_progress

# Special date views
./task_tracker list --due-today
./task_tracker list --overdue
```

#### Search Functionality
```bash
# Search with keyword
./task_tracker search "meeting"

# Search with filters
./task_tracker search "report" --status pending
./task_tracker search "project" --priority high
```

#### Date-based Commands
```bash
# View tasks due today
./task_tracker due today
./task_tracker today

# View tasks due by specific date
./task_tracker due 2025-06-15

# View overdue tasks
./task_tracker overdue
```

#### Enhanced Updates
```bash
# Update multiple attributes
./task_tracker update 1 --priority high --due 2025-06-20 --status in_progress

# Update description
./task_tracker update 1 --description "Updated task description"

# Legacy support (still works)
./task_tracker update 1 done
```

#### Filter Commands
```bash
# Filter by priority
./task_tracker filter priority high

# Filter by status
./task_tracker filter status pending
```

### Example Enhanced Session

```bash
$ ./task_tracker add "Prepare presentation" --priority high --due 2025-06-10
Task added successfully with ID: 1 (Priority: high) (Due: 2025-06-10)

$ ./task_tracker add "Buy groceries" --priority medium --due 2025-06-08
Task added successfully with ID: 2 (Priority: medium) (Due: 2025-06-08)

$ ./task_tracker add "Read documentation" --priority low
Task added successfully with ID: 3

$ ./task_tracker list

All Tasks:
==========
ID  P  Description              Status         Due Date    Days Left 
----------------------------------------------------------------------
1   [!] Prepare presentation     [ ] pending    2025-06-10  7 days    
2   [>] Buy groceries           [ ] pending    2025-06-08  5 days    
3   [-] Read documentation      [ ] pending    -           -         

$ ./task_tracker list --priority high

All Tasks (Priority: high):
==========================
ID  P  Description              Status         Due Date    Days Left 
----------------------------------------------------------------------
1   [!] Prepare presentation     [ ] pending    2025-06-10  7 days    

$ ./task_tracker search "present"

Search Results for: "present":
==============================
ID  P  Description              Status         Due Date    Days Left 
----------------------------------------------------------------------
1   [!] Prepare presentation     [ ] pending    2025-06-10  7 days    

$ ./task_tracker progress 1
Task 1 updated successfully.

$ ./task_tracker overdue
Overdue Tasks:
==============
No tasks found.
```

## Enhanced Data Storage

Tasks now include priority, due dates, and creation dates in the JSON format:

```json
[
  {
    "id": 1,
    "description": "Prepare presentation",
    "status": "in_progress",
    "priority": "high",
    "due_date": "2025-06-10",
    "created_date": "2025-06-03"
  },
  {
    "id": 2,
    "description": "Buy groceries",
    "status": "pending",
    "priority": "medium",
    "due_date": "2025-06-08",
    "created_date": "2025-06-03"
  }
]
```

## Enhanced Output Format

The new table format includes:
- **Priority indicators**: `[!]` High, `[>]` Medium, `[-]` Low
- **Status indicators**: `[X]` Done, `[>]` In Progress, `[ ]` Pending
- **Due date information**: Shows actual due dates
- **Days remaining**: Calculates days until due, shows "OVERDUE" or "TODAY" as appropriate

## Testing

Run the enhanced unit tests:

```bash
cd build
ctest --verbose
```

New test files include:
- `test_date_utils.cpp` - Tests for date validation and calculations
- `test_enhanced_task.cpp` - Tests for new Task features

## Command Reference

### Basic Commands
- `add "description" [options]` - Add new task
- `list [filters]` - List tasks with optional filters
- `update <id> [options]` - Update task attributes
- `delete <id>` - Delete task
- `done <id>` - Mark as done
- `progress <id>` - Mark as in progress

### Enhanced Commands
- `search "keyword" [filters]` - Search by keyword
- `filter <type> <value>` - Filter by criteria
- `due <date|today>` - Show tasks due by date
- `overdue` - Show overdue tasks
- `today` - Show tasks due today

### Options
- `--priority, -p <high|medium|low>` - Set/filter by priority
- `--due, -d <YYYY-MM-DD>` - Set/filter by due date
- `--status <pending|in_progress|done>` - Filter by status
- `--description "text"` - Update description

## Architecture Changes

### New Components
- **DateUtils namespace**: Date validation, parsing, and calculations
- **Enhanced Task class**: Added priority, due_date, created_date fields
- **Enhanced TaskManager**: New filtering and search methods
- **Enhanced CommandHandler**: Support for options and advanced commands

### Updated Components
- **JSONParser**: Handles new task attributes
- **Command processing**: Flexible argument parsing with options
- **Display formatting**: Rich table output with indicators

## Future Enhancements

Phase 3 will include:
- Advanced filtering and sorting combinations
- Interactive mode for seamless task management
- Enhanced date range filtering
- Performance optimizations

## License

This project is for educational purposes.