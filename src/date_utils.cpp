#include "date_utils.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <regex>

namespace DateUtils {

bool isValidDate(const std::string& date_str) {
    int year, month, day;
    if (!parseDateString(date_str, year, month, day)) {
        return false;
    }
    
    // Check basic ranges
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    
    // Check days in month
    int daysInMonth = getDaysInMonth(month, year);
    return day <= daysInMonth;
}

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d");
    return ss.str();
}

std::string formatDate(const std::string& date_str) {
    // For now, just return the input as we're using ISO format
    return date_str;
}

int daysBetween(const std::string& date1_str, const std::string& date2_str) {
    int year1, month1, day1, year2, month2, day2;
    
    if (!parseDateString(date1_str, year1, month1, day1) ||
        !parseDateString(date2_str, year2, month2, day2)) {
        return 0;
    }
    
    // Convert to days since epoch (simplified calculation)
    auto date1 = std::chrono::year_month_day{std::chrono::year{year1}, 
                                           std::chrono::month{static_cast<unsigned>(month1)}, 
                                           std::chrono::day{static_cast<unsigned>(day1)}};
    auto date2 = std::chrono::year_month_day{std::chrono::year{year2}, 
                                           std::chrono::month{static_cast<unsigned>(month2)}, 
                                           std::chrono::day{static_cast<unsigned>(day2)}};
    
    // Simple day calculation (not accounting for all edge cases)
    int days1 = year1 * 365 + month1 * 30 + day1;
    int days2 = year2 * 365 + month2 * 30 + day2;
    
    return days2 - days1;
}

bool isDateBefore(const std::string& date1_str, const std::string& date2_str) {
    return daysBetween(date1_str, date2_str) > 0;
}

bool isDateEqual(const std::string& date1_str, const std::string& date2_str) {
    return date1_str == date2_str;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getDaysInMonth(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

bool parseDateString(const std::string& date_str, int& year, int& month, int& day) {
    // Expected format: YYYY-MM-DD
    std::regex date_regex(R"((\d{4})-(\d{2})-(\d{2}))");
    std::smatch matches;
    
    if (!std::regex_match(date_str, matches, date_regex)) {
        return false;
    }
    
    try {
        year = std::stoi(matches[1]);
        month = std::stoi(matches[2]);
        day = std::stoi(matches[3]);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

}