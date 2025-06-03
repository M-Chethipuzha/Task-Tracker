#include <iostream>
#include <cassert>
#include "../src/json_parser.h"
#include "../src/task.h"

void testJSONEscaping() {
    std::cout << "Testing JSON escaping..." << std::endl;
    
    std::string input = "Test \"quoted\" text with \n newline";
    std::string escaped = JSONParser::escapeJSON(input);
    std::string unescaped = JSONParser::unescapeJSON(escaped);
    
    assert(unescaped == input);
    
    std::cout << "✓ JSON escaping tests passed!" << std::endl;
}

void testJSONSerialization() {
    std::cout << "Testing JSON serialization..." << std::endl;
    
    std::vector<Task> tasks;
    tasks.emplace_back(1, "Test task 1", "pending");
    tasks.emplace_back(2, "Test task 2", "done");
    
    std::string json = JSONParser::tasksToJSON(tasks);
    
    // Basic checks that JSON contains expected content
    assert(json.find("\"id\": 1") != std::string::npos);
    assert(json.find("\"id\": 2") != std::string::npos);
    assert(json.find("Test task 1") != std::string::npos);
    assert(json.find("Test task 2") != std::string::npos);
    assert(json.find("pending") != std::string::npos);
    assert(json.find("done") != std::string::npos);
    
    std::cout << "✓ JSON serialization tests passed!" << std::endl;
}

void testJSONDeserialization() {
    std::cout << "Testing JSON deserialization..." << std::endl;
    
    std::string json = R"([
  {
    "id": 1,
    "description": "Test task 1",
    "status": "pending"
  },
  {
    "id": 2,
    "description": "Test task 2",
    "status": "done"
  }
])";
    
    std::vector<Task> tasks = JSONParser::parseTasksFromJSON(json);
    
    assert(tasks.size() == 2);
    assert(tasks[0].getId() == 1);
    assert(tasks[0].getDescription() == "Test task 1");
    assert(tasks[0].getStatus() == "pending");
    assert(tasks[1].getId() == 2);
    assert(tasks[1].getDescription() == "Test task 2");
    assert(tasks[1].getStatus() == "done");
    
    std::cout << "✓ JSON deserialization tests passed!" << std::endl;
}

void testRoundTripSerialization() {
    std::cout << "Testing round-trip serialization..." << std::endl;
    
    std::vector<Task> originalTasks;
    originalTasks.emplace_back(1, "Buy groceries", "pending");
    originalTasks.emplace_back(2, "Write report", "in_progress");
    originalTasks.emplace_back(3, "Call client", "done");
    
    std::string json = JSONParser::tasksToJSON(originalTasks);
    std::vector<Task> parsedTasks = JSONParser::parseTasksFromJSON(json);
    
    assert(parsedTasks.size() == originalTasks.size());
    for (size_t i = 0; i < originalTasks.size(); ++i) {
        assert(parsedTasks[i].getId() == originalTasks[i].getId());
        assert(parsedTasks[i].getDescription() == originalTasks[i].getDescription());
        assert(parsedTasks[i].getStatus() == originalTasks[i].getStatus());
    }
    
    std::cout << "✓ Round-trip serialization tests passed!" << std::endl;
}

int main() {
    std::cout << "Running JSONParser unit tests...\n" << std::endl;
    
    testJSONEscaping();
    testJSONSerialization();
    testJSONDeserialization();
    testRoundTripSerialization();
    
    std::cout << "\n✓ All JSONParser tests passed!" << std::endl;
    return 0;
}