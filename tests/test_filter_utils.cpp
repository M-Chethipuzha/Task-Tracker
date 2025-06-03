#include <iostream>
#include <cassert>
#include "../src/filter_utils.h"
#include "../src/task.h"
#include "../src/date_utils.h"

void testPriorityFiltering()
{
    std::cout << "Testing priority filtering..." << std::endl;

    std::vector<Task> tasks;
    tasks.emplace_back(1, "High priority task", "high");
    tasks.emplace_back(2, "Medium priority task", "medium");
    tasks.emplace_back(3, "Low priority task", "low");
    tasks.emplace_back(4, "Another high priority", "high");

    auto highTasks = FilterUtils::filterByPriority(tasks, "high");
    assert(highTasks.size() == 2);
    assert(highTasks[0].getPriority() == "high");
    assert(highTasks[1].getPriority() == "high");

    auto mediumTasks = FilterUtils::filterByPriority(tasks, "medium");
    assert(mediumTasks.size() == 1);
    assert(mediumTasks[0].getPriority() == "medium");

    std::cout << "✓ Priority filtering tests passed!" << std::endl;
}

void testKeywordFiltering()
{
    std::cout << "Testing keyword filtering..." << std::endl;

    std::vector<Task> tasks;
    tasks.emplace_back(1, "Buy groceries", "medium");
    tasks.emplace_back(2, "Write meeting notes", "high");
    tasks.emplace_back(3, "Schedule doctor appointment", "low");
    tasks.emplace_back(4, "Grocery shopping list", "medium");

    auto groceryTasks = FilterUtils::filterByKeyword(tasks, "grocery");
    assert(groceryTasks.size() == 2); // Case insensitive matching

    auto meetingTasks = FilterUtils::filterByKeyword(tasks, "meeting");
    assert(meetingTasks.size() == 1);

    std::cout << "✓ Keyword filtering tests passed!" << std::endl;
}

void testSorting()
{
    std::cout << "Testing sorting..." << std::endl;

    std::vector<Task> tasks;
    tasks.emplace_back(3, "Low priority task", "low");
    tasks.emplace_back(1, "High priority task", "high");
    tasks.emplace_back(2, "Medium priority task", "medium");

    // Test priority sorting (descending - high to low)
    auto sortedByPriority = FilterUtils::sortByPriority(tasks, false);
    assert(sortedByPriority[0].getPriority() == "high");
    assert(sortedByPriority[1].getPriority() == "medium");
    assert(sortedByPriority[2].getPriority() == "low");

    // Test ID sorting
    auto sortedById = FilterUtils::sortById(tasks, true);
    assert(sortedById[0].getId() == 1);
    assert(sortedById[1].getId() == 2);
    assert(sortedById[2].getId() == 3);

    std::cout << "✓ Sorting tests passed!" << std::endl;
}

void testCombinedFiltering()
{
    std::cout << "Testing combined filtering..." << std::endl;

    std::vector<Task> tasks;
    tasks.emplace_back(1, "High priority meeting", "high");
    tasks.emplace_back(2, "Medium priority meeting", "medium");
    tasks.emplace_back(3, "High priority shopping", "high");
    tasks.emplace_back(4, "Low priority meeting", "low");

    // Filter by keyword and priority
    auto results = FilterUtils::filterByCriteria(tasks, "meeting", "high");
    assert(results.size() == 1);
    assert(results[0].getId() == 1);

    std::cout << "✓ Combined filtering tests passed!" << std::endl;
}

void testValidation()
{
    std::cout << "Testing validation functions..." << std::endl;

    assert(FilterUtils::isValidSortField("priority") == true);
    assert(FilterUtils::isValidSortField("due_date") == true);
    assert(FilterUtils::isValidSortField("status") == true);
    assert(FilterUtils::isValidSortField("invalid_field") == false);

    assert(FilterUtils::getPriorityValue("high") == 3);
    assert(FilterUtils::getPriorityValue("medium") == 2);
    assert(FilterUtils::getPriorityValue("low") == 1);

    std::cout << "✓ Validation tests passed!" << std::endl;
}

int main()
{
    std::cout << "Running FilterUtils unit tests...\n"
              << std::endl;

    testPriorityFiltering();
    testKeywordFiltering();
    testSorting();
    testCombinedFiltering();
    testValidation();

    std::cout << "\n✓ All FilterUtils tests passed!" << std::endl;
    return 0;
}