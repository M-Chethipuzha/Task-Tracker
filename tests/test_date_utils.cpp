#include <iostream>
#include <cassert>
#include "../src/date_utils.h"

void testDateValidation() {
    std::cout << "Testing date validation..." << std::endl;
    
    // Valid dates
    assert(DateUtils::isValidDate("2025-06-15") == true);
    assert(DateUtils::isValidDate("2024-02-29") == true);  // Leap year
    assert(DateUtils::isValidDate("2025-12-31") == true);
    
    // Invalid dates
    assert(DateUtils::isValidDate("2025-13-01") == false); // Invalid month
    assert(DateUtils::isValidDate("2025-06-32") == false); // Invalid day
    assert(DateUtils::isValidDate("2025-02-29") == false); // Not a leap year
    assert(DateUtils::isValidDate("25-06-15") == false);   // Wrong format
    assert(DateUtils::isValidDate("invalid") == false);    // Invalid format
    
    std::cout << "✓ Date validation tests passed!" << std::endl;
}

void testDateComparisons() {
    std::cout << "Testing date comparisons..." << std::endl;
    
    assert(DateUtils::isDateBefore("2025-06-01", "2025-06-02") == true);
    assert(DateUtils::isDateBefore("2025-06-02", "2025-06-01") == false);
    assert(DateUtils::isDateEqual("2025-06-15", "2025-06-15") == true);
    assert(DateUtils::isDateEqual("2025-06-15", "2025-06-16") == false);
    
    std::cout << "✓ Date comparison tests passed!" << std::endl;
}

void testLeapYear() {
    std::cout << "Testing leap year calculation..." << std::endl;
    
    assert(DateUtils::isLeapYear(2024) == true);
    assert(DateUtils::isLeapYear(2025) == false);
    assert(DateUtils::isLeapYear(2000) == true);
    assert(DateUtils::isLeapYear(1900) == false);
    
    std::cout << "✓ Leap year tests passed!" << std::endl;
}

void testDaysInMonth() {
    std::cout << "Testing days in month calculation..." << std::endl;
    
    assert(DateUtils::getDaysInMonth(1, 2025) == 31);  // January
    assert(DateUtils::getDaysInMonth(2, 2024) == 29);  // February (leap year)
    assert(DateUtils::getDaysInMonth(2, 2025) == 28);  // February (non-leap year)
    assert(DateUtils::getDaysInMonth(4, 2025) == 30);  // April
    assert(DateUtils::getDaysInMonth(12, 2025) == 31); // December
    
    std::cout << "✓ Days in month tests passed!" << std::endl;
}

int main() {
    std::cout << "Running DateUtils unit tests...\n" << std::endl;
    
    testDateValidation();
    testDateComparisons();
    testLeapYear();
    testDaysInMonth();
    
    std::cout << "\n✓ All DateUtils tests passed!" << std::endl;
    return 0;
}