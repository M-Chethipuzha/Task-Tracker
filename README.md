# Task Tracker - Phase 1 (MVP)

A command-line interface (CLI) application for managing daily tasks efficiently.

## Features

- Add new tasks with descriptions
- List all tasks or filter by status (pending, in_progress, done)
- Update task status
- Delete tasks
- Mark tasks as done or in progress
- JSON-based persistence

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

### Basic Commands

```bash
# Add a new task
./task_tracker add "Buy groceries"

# List all tasks
./task_tracker list

# List tasks by status
./task_tracker list pending
./task_tracker list in_progress
./task_tracker list done

# Update task status
./task_tracker update 1 done

# Mark task as done (shortcut)
./task_tracker done 1

# Mark task as in progress (shortcut)
./task_tracker progress 1

# Delete a task
./task_tracker delete 1

# Show help
./task_tracker help
```

### Example Session

```bash
$ ./task_tracker add "Buy groceries"
Task added successfully with ID: 1

$ ./task_tracker add "Write report"
Task added successfully with ID: 2

$ ./task_tracker list

All Tasks:
==========
ID   Description                    Status         
--------------------------------------------------
1    Buy groceries                  [ ] pending    
2    Write report                   [ ] pending    

$ ./task_tracker progress 1
Task 1 updated successfully.

$ ./task_tracker done 2
Task 2 updated successfully.

$ ./task_tracker list

All Tasks:
==========
ID   Description                    Status         
--------------------------------------------------
1    Buy groceries                  [>] in_progress
2    Write report                   [X] done       
```

## Data Storage

Tasks are automatically saved to `tasks.json` in the current working directory. The JSON format is:

```json
[
  {
    "id": 1,
    "description": "Buy groceries",
    "status": "pending"
  },
  {
    "id": 2,
    "description": "Write report",
    "status": "done"
  }
]
```

## Testing

Run the unit tests to verify functionality:

```bash
cd build
ctest --verbose
```

## Project Structure

```
src/
├── main.cpp              # Entry point and CLI argument parsing
├── task.h/cpp            # Task class implementation
├── task_manager.h/cpp    # Task collection management
├── json_parser.h/cpp     # JSON serialization/deserialization
└── command_handler.h/cpp # Command processing and execution

tests/
├── test_task.cpp         # Unit tests for Task class
├── test_json_parser.cpp  # Unit tests for JSONParser
└── CMakeLists.txt        # Test configuration

CMakeLists.txt            # Main build configuration
README.md                 # This file
```

## Future Enhancements

This is Phase 1 (MVP) of the Task Tracker. Future phases will include:

- Priority levels (high, medium, low)
- Due dates and date-based filtering
- Advanced search and filtering capabilities
- Sorting options
- Interactive mode
- Enhanced CLI interface

## License

This project is for educational purposes.