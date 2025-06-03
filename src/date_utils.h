#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <string>

namespace DateUtils
{
    bool isValidDate(const std::string &date_str);
    std::string getCurrentDate();
    std::string formatDate(const std::string &date_str);
    int daysBetween(const std::string &date1_str, const std::string &date2_str);
    bool isDateBefore(const std::string &date1_str, const std::string &date2_str);
    bool isDateEqual(const std::string &date1_str, const std::string &date2_str);

    // Helper functions
    bool isLeapYear(int year);
    int getDaysInMonth(int month, int year);
    bool parseDateString(const std::string &date_str, int &year, int &month, int &day);

    // Date range utilities
    bool isDateInRange(const std::string &date_str, const std::string &start_date, const std::string &end_date);
    std::string addDays(const std::string &date_str, int days);
}

#endif