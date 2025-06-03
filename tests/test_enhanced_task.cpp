#include <iostream>
#include <cassert>
#include "../src/task.h"
#include "../src/date_utils.h"

void testEnhancedTaskCreation() {
    std::cout << "Testing enhanced Task creation..." << std::endl;
    
    // Test with priority and due date
    Task task(1, "Test task", "high", "2025-06-15");
    assert(task.getId() == 1);
    assert(task.getDescription() == "Test task");
    assert(task.getPriority() == "high");
    assert(task.getDueDate() == "2025-06-15");
    assert(!task.getCreatedDate().empty());
    
    // Test invalid priority handling
    Task task2(2, "Test task", "invalid_priority");
    assert(task2.getPriority() == "medium"); // Should default to medium
    
    std::cout << "✓ Enhanced Task creation tests passed!" << std::endl;
}

void testTaskValidations() {
    std::cout << "Testing Task validations..." << std::endl;
    
    Task task;
    
    // Priority validation
    assert(task.isValidPriority("high") == true);
    assert(task.isValidPriority("medium") == true);
    assert(task.isValidPriority("low") == true);
    assert(task.isValidPriority("invalid") == false);
    
    // Status validation (existing)
    assert(task.isValidStatus("pending") == true);
    assert(task.isValidStatus("in_progress") == true);
    assert(task.isValidStatus("done") == true);
    assert(task.isValidStatus("invalid") == false);
    
    std::cout << "✓ Task validation tests passed!" << std::endl;
}

void testTaskUtilities() {
    std::cout << "Testing Task utility methods..." << std::endl;
    
    std::string today = DateUtils::getCurrentDate();
    
    // Test due today
    Task taskDueToday(1, "Due today", "medium", today);
    assert(taskDueToday.isDueToday() == true);
    
    // Test keyword matching
    Task task(2, "Buy groceries at the store", "medium");
    assert(task.matchesKeyword("grocery") == true);
    assert(task.matchesKeyword("GROCERY") == true); // Case insensitive
    assert(task.matchesKeyword("store") == true);
    assert(task.matchesKeyword("xyz") == false);
    
    std::cout << "✓ Task utility tests passed!" << std::endl;
}

int main() {
    std::cout << "Running Enhanced Task unit tests...\n" << std::endl;
    
    testEnhancedTaskCreation();
    testTaskValidations();
    testTaskUtilities();
    
    std::cout << "\n✓ All Enhanced Task tests passed!" << std::endl;
    return 0;
}