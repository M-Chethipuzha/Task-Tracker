cmake_minimum_required(VERSION 3.16)

# Project configuration
project(TaskTracker VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Debug and Release configurations
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Source files
set(SOURCES
    src/main.cpp
    src/task.cpp
    src/task_manager.cpp
    src/json_parser.cpp
    src/command_handler.cpp
    src/date_utils.cpp
)

# Create executable
add_executable(task_tracker ${SOURCES})

# Include directories (for headers)
target_include_directories(task_tracker PRIVATE src)

# Set output directory
set_target_properties(task_tracker PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation (optional)
install(TARGETS task_tracker
    RUNTIME DESTINATION bin
)