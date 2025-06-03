# Test configuration
enable_testing()

# Test executable for Task class
add_executable(test_task
    test_task.cpp
    ../src/task.cpp
    ../src/date_utils.cpp
)
target_include_directories(test_task PRIVATE ../src)

# Test executable for JSONParser
add_executable(test_json_parser
    test_json_parser.cpp
    ../src/task.cpp
    ../src/json_parser.cpp
    ../src/date_utils.cpp
)
target_include_directories(test_json_parser PRIVATE ../src)

# Test executable for DateUtils
add_executable(test_date_utils
    test_date_utils.cpp
    ../src/date_utils.cpp
)
target_include_directories(test_date_utils PRIVATE ../src)

# Test executable for Enhanced Task features
add_executable(test_enhanced_task
    test_enhanced_task.cpp
    ../src/task.cpp
    ../src/date_utils.cpp
)
target_include_directories(test_enhanced_task PRIVATE ../src)

# Test executable for FilterUtils
add_executable(test_filter_utils
    test_filter_utils.cpp
    ../src/filter_utils.cpp
    ../src/task.cpp
    ../src/date_utils.cpp
)
target_include_directories(test_filter_utils PRIVATE ../src)

# Add tests
add_test(NAME TaskTests COMMAND test_task)
add_test(NAME JSONParserTests COMMAND test_json_parser)
add_test(NAME DateUtilsTests COMMAND test_date_utils)
add_test(NAME EnhancedTaskTests COMMAND test_enhanced_task)
add_test(NAME FilterUtilsTests COMMAND test_filter_utils)