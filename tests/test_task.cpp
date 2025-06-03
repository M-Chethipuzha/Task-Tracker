#include <iostream>
#include <cassert>
#include "../src/task.h"

void testTaskCreation() {
    std::cout << "Testing Task creation..." << std::endl;
    
    // Test default constructor
    Task task1;
    assert(task1.getId() == 0);
    assert(task1.getDescription() == "");
    assert(task1.getStatus() == "pending");
    
    // Test parameterized constructor
    Task task2(1, "Test task", "in_progress");
    assert(task2.getId() == 1);
    assert(task2.getDescription() == "Test task");
    assert(task2.getStatus() == "in_progress");
    
    // Test invalid status handling
    Task task3(2, "Test task", "invalid_status");
    assert(task3.getStatus() == "pending"); // Should default to pending
    
    std::cout << "✓ Task creation tests passed!" << std::endl;
}

void testTaskSetters() {
    std::cout << "Testing Task setters..." << std::endl;
    
    Task task;
    
    task.setId(5);
    assert(task.getId() == 5);
    
    task.setDescription("New description");
    assert(task.getDescription() == "New description");
    
    task.setStatus("done");
    assert(task.getStatus() == "done");
    
    // Test invalid status
    task.setStatus("invalid");
    assert(task.getStatus() == "pending"); // Should revert to pending
    
    std::cout << "✓ Task setter tests passed!" << std::endl;
}

void testTaskValidation() {
    std::cout << "Testing Task validation..." << std::endl;
    
    Task task;
    
    assert(task.isValidStatus("pending") == true);
    assert(task.isValidStatus("in_progress") == true);
    assert(task.isValidStatus("done") == true);
    assert(task.isValidStatus("invalid") == false);
    assert(task.isValidStatus("") == false);
    
    std::cout << "✓ Task validation tests passed!" << std::endl;
}

int main() {
    std::cout << "Running Task unit tests...\n" << std::endl;
    
    testTaskCreation();
    testTaskSetters();
    testTaskValidation();
    
    std::cout << "\n✓ All Task tests passed!" << std::endl;
    return 0;
}