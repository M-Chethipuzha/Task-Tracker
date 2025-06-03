#include "filter_utils.h"
#include "date_utils.h"
#include <algorithm>
#include <iostream>

namespace FilterUtils
{

    std::vector<Task> filterByKeyword(const std::vector<Task> &tasks, const std::string &keyword)
    {
        if (keyword.empty())
            return tasks;

        std::vector<Task> filtered;
        for (const Task &task : tasks)
        {
            if (task.matchesKeyword(keyword))
            {
                filtered.push_back(task);
            }
        }
        return filtered;
    }

    std::vector<Task> filterByPriority(const std::vector<Task> &tasks, const std::string &priority)
    {
        if (priority.empty())
            return tasks;

        std::vector<Task> filtered;
        for (const Task &task : tasks)
        {
            if (task.getPriority() == priority)
            {
                filtered.push_back(task);
            }
        }
        return filtered;
    }

    std::vector<Task> filterByStatus(const std::vector<Task> &tasks, const std::string &status)
    {
        if (status.empty())
            return tasks;

        std::vector<Task> filtered;
        for (const Task &task : tasks)
        {
            if (task.getStatus() == status)
            {
                filtered.push_back(task);
            }
        }
        return filtered;
    }

    std::vector<Task> filterByDateRange(const std::vector<Task> &tasks,
                                        const std::string &start_date, const std::string &end_date)
    {
        if (start_date.empty() || end_date.empty())
            return tasks;

        std::vector<Task> filtered;
        for (const Task &task : tasks)
        {
            if (!task.getDueDate().empty() &&
                DateUtils::isDateInRange(task.getDueDate(), start_date, end_date))
            {
                filtered.push_back(task);
            }
        }
        return filtered;
    }

    std::vector<Task> filterByDueDate(const std::vector<Task> &tasks, const std::string &due_date)
    {
        if (due_date.empty())
            return tasks;

        std::vector<Task> filtered;
        for (const Task &task : tasks)
        {
            if (task.getDueDate() == due_date)
            {
                filtered.push_back(task);
            }
        }
        return filtered;
    }

    std::vector<Task> filterOverdue(const std::vector<Task> &tasks)
    {
        std::vector<Task> filtered;
        for (const Task &task : tasks)
        {
            if (task.isOverdue())
            {
                filtered.push_back(task);
            }
        }
        return filtered;
    }

    std::vector<Task> filterDueToday(const std::vector<Task> &tasks)
    {
        std::vector<Task> filtered;
        for (const Task &task : tasks)
        {
            if (task.isDueToday())
            {
                filtered.push_back(task);
            }
        }
        return filtered;
    }

    std::vector<Task> filterByCriteria(const std::vector<Task> &tasks,
                                       const std::string &keyword,
                                       const std::string &priority,
                                       const std::string &status,
                                       const std::string &due_date,
                                       bool overdue_only,
                                       bool due_today_only)
    {
        std::vector<Task> result = tasks;

        // Apply filters in sequence
        if (!keyword.empty())
        {
            result = filterByKeyword(result, keyword);
        }

        if (!priority.empty())
        {
            result = filterByPriority(result, priority);
        }

        if (!status.empty())
        {
            result = filterByStatus(result, status);
        }

        if (!due_date.empty())
        {
            result = filterByDueDate(result, due_date);
        }

        if (overdue_only)
        {
            result = filterOverdue(result);
        }

        if (due_today_only)
        {
            result = filterDueToday(result);
        }

        return result;
    }

    std::vector<Task> sortByPriority(std::vector<Task> tasks, bool ascending)
    {
        std::sort(tasks.begin(), tasks.end(), [ascending](const Task &a, const Task &b)
                  {
        int priorityA = getPriorityValue(a.getPriority());
        int priorityB = getPriorityValue(b.getPriority());
        return ascending ? priorityA < priorityB : priorityA > priorityB; });
        return tasks;
    }

    std::vector<Task> sortByDueDate(std::vector<Task> tasks, bool ascending)
    {
        std::sort(tasks.begin(), tasks.end(), [ascending](const Task &a, const Task &b)
                  {
        std::string dateA = a.getDueDate();
        std::string dateB = b.getDueDate();
        
        // Handle empty dates (put them at the end)
        if (dateA.empty() && dateB.empty()) return false;
        if (dateA.empty()) return false;
        if (dateB.empty()) return true;
        
        if (ascending) {
            return DateUtils::isDateBefore(dateA, dateB);
        } else {
            return DateUtils::isDateBefore(dateB, dateA);
        } });
        return tasks;
    }

    std::vector<Task> sortByStatus(std::vector<Task> tasks, bool ascending)
    {
        std::sort(tasks.begin(), tasks.end(), [ascending](const Task &a, const Task &b)
                  {
        int statusA = getStatusValue(a.getStatus());
        int statusB = getStatusValue(b.getStatus());
        return ascending ? statusA < statusB : statusA > statusB; });
        return tasks;
    }

    std::vector<Task> sortById(std::vector<Task> tasks, bool ascending)
    {
        std::sort(tasks.begin(), tasks.end(), [ascending](const Task &a, const Task &b)
                  { return ascending ? a.getId() < b.getId() : a.getId() > b.getId(); });
        return tasks;
    }

    std::vector<Task> sortByCreatedDate(std::vector<Task> tasks, bool ascending)
    {
        std::sort(tasks.begin(), tasks.end(), [ascending](const Task &a, const Task &b)
                  {
        if (ascending) {
            return DateUtils::isDateBefore(a.getCreatedDate(), b.getCreatedDate());
        } else {
            return DateUtils::isDateBefore(b.getCreatedDate(), a.getCreatedDate());
        } });
        return tasks;
    }

    std::vector<Task> filterAndSort(const std::vector<Task> &tasks,
                                    const std::string &sort_by,
                                    bool ascending,
                                    const std::string &keyword,
                                    const std::string &priority,
                                    const std::string &status,
                                    const std::string &due_date,
                                    bool overdue_only,
                                    bool due_today_only)
    {
        // First apply filters
        std::vector<Task> result = filterByCriteria(tasks, keyword, priority, status,
                                                    due_date, overdue_only, due_today_only);

        // Then apply sorting
        if (!sort_by.empty())
        {
            if (sort_by == "priority")
            {
                result = sortByPriority(result, ascending);
            }
            else if (sort_by == "due_date" || sort_by == "due")
            {
                result = sortByDueDate(result, ascending);
            }
            else if (sort_by == "status")
            {
                result = sortByStatus(result, ascending);
            }
            else if (sort_by == "id")
            {
                result = sortById(result, ascending);
            }
            else if (sort_by == "created_date" || sort_by == "created")
            {
                result = sortByCreatedDate(result, ascending);
            }
        }

        return result;
    }

    int getPriorityValue(const std::string &priority)
    {
        if (priority == "high")
            return 3;
        if (priority == "medium")
            return 2;
        if (priority == "low")
            return 1;
        return 0;
    }

    int getStatusValue(const std::string &status)
    {
        if (status == "pending")
            return 1;
        if (status == "in_progress")
            return 2;
        if (status == "done")
            return 3;
        return 0;
    }

    bool isValidSortField(const std::string &field)
    {
        return (field == "priority" || field == "due_date" || field == "due" ||
                field == "status" || field == "id" || field == "created_date" || field == "created");
    }

}