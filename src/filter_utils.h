#ifndef FILTER_UTILS_H
#define FILTER_UTILS_H

#include <vector>
#include <string>
#include <functional>
#include "task.h"

namespace FilterUtils
{
    // Filtering functions
    std::vector<Task> filterByKeyword(const std::vector<Task> &tasks, const std::string &keyword);
    std::vector<Task> filterByPriority(const std::vector<Task> &tasks, const std::string &priority);
    std::vector<Task> filterByStatus(const std::vector<Task> &tasks, const std::string &status);
    std::vector<Task> filterByDateRange(const std::vector<Task> &tasks,
                                        const std::string &start_date, const std::string &end_date);
    std::vector<Task> filterByDueDate(const std::vector<Task> &tasks, const std::string &due_date);
    std::vector<Task> filterOverdue(const std::vector<Task> &tasks);
    std::vector<Task> filterDueToday(const std::vector<Task> &tasks);

    // Advanced filtering with multiple criteria
    std::vector<Task> filterByCriteria(const std::vector<Task> &tasks,
                                       const std::string &keyword = "",
                                       const std::string &priority = "",
                                       const std::string &status = "",
                                       const std::string &due_date = "",
                                       bool overdue_only = false,
                                       bool due_today_only = false);

    // Sorting functions
    std::vector<Task> sortByPriority(std::vector<Task> tasks, bool ascending = false);
    std::vector<Task> sortByDueDate(std::vector<Task> tasks, bool ascending = true);
    std::vector<Task> sortByStatus(std::vector<Task> tasks, bool ascending = true);
    std::vector<Task> sortById(std::vector<Task> tasks, bool ascending = true);
    std::vector<Task> sortByCreatedDate(std::vector<Task> tasks, bool ascending = true);

    // Combined filter and sort
    std::vector<Task> filterAndSort(const std::vector<Task> &tasks,
                                    const std::string &sort_by = "",
                                    bool ascending = true,
                                    const std::string &keyword = "",
                                    const std::string &priority = "",
                                    const std::string &status = "",
                                    const std::string &due_date = "",
                                    bool overdue_only = false,
                                    bool due_today_only = false);

    // Utility functions
    int getPriorityValue(const std::string &priority);
    int getStatusValue(const std::string &status);
    bool isValidSortField(const std::string &field);
}

#endif