# Test configuration
enable_testing()

# Test executable for Task class
add_executable(test_task
    test_task.cpp
    ../src/task.cpp
)
target_include_directories(test_task PRIVATE ../src)

# Test executable for JSONParser
add_executable(test_json_parser
    test_json_parser.cpp
    ../src/task.cpp
    ../src/json_parser.cpp
)
target_include_directories(test_json_parser PRIVATE ../src)

# Add tests
add_test(NAME TaskTests COMMAND test_task)
add_test(NAME JSONParserTests COMMAND test_json_parser)