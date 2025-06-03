#include "task_manager.h"
#include "json_parser.h"
#include "date_utils.h"
#include "filter_utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

TaskManager::TaskManager(const std::string &filename)
    : filename(filename), nextId(1)
{
    loadTasks();
}

void TaskManager::addTask(const std::string &description, const std::string &priority,
                          const std::string &due_date)
{
    if (description.empty())
    {
        std::cerr << "Error: Task description cannot be empty." << std::endl;
        return;
    }

    int id = generateNextId();
    Task task(id, description, priority, due_date);
    tasks.push_back(task);
    saveTasks();

    std::cout << "Task added successfully with ID: " << id;
    if (!priority.empty() && priority != "medium")
    {
        std::cout << " (Priority: " << priority << ")";
    }
    if (!due_date.empty())
    {
        std::cout << " (Due: " << due_date << ")";
    }
    std::cout << std::endl;
}

bool TaskManager::updateTask(int id, const std::string &description,
                             const std::string &status, const std::string &priority,
                             const std::string &due_date)
{
    Task *task = findTaskById(id);
    if (task == nullptr)
    {
        std::cerr << "Error: Task with ID " << id << " not found." << std::endl;
        return false;
    }

    bool updated = false;

    if (!description.empty())
    {
        task->setDescription(description);
        updated = true;
    }

    if (!status.empty())
    {
        if (!task->isValidStatus(status))
        {
            std::cerr << "Error: Invalid status '" << status << "'. Valid statuses are: pending, in_progress, done" << std::endl;
            return false;
        }
        task->setStatus(status);
        updated = true;
    }

    if (!priority.empty())
    {
        if (!task->isValidPriority(priority))
        {
            std::cerr << "Error: Invalid priority '" << priority << "'. Valid priorities are: high, medium, low" << std::endl;
            return false;
        }
        task->setPriority(priority);
        updated = true;
    }

    if (!due_date.empty())
    {
        if (!DateUtils::isValidDate(due_date))
        {
            std::cerr << "Error: Invalid date format '" << due_date << "'. Please use YYYY-MM-DD format." << std::endl;
            return false;
        }
        task->setDueDate(due_date);
        updated = true;
    }

    if (updated)
    {
        saveTasks();
        std::cout << "Task " << id << " updated successfully." << std::endl;
    }

    return updated;
}

bool TaskManager::deleteTask(int id)
{
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [id](const Task &task)
                           { return task.getId() == id; });

    if (it == tasks.end())
    {
        std::cerr << "Error: Task with ID " << id << " not found." << std::endl;
        return false;
    }

    tasks.erase(it);
    saveTasks();
    std::cout << "Task " << id << " deleted successfully." << std::endl;
    return true;
}

std::vector<Task> TaskManager::getAllTasks() const
{
    return tasks;
}

std::vector<Task> TaskManager::getTasksByStatus(const std::string &status) const
{
    return FilterUtils::filterByStatus(tasks, status);
}

std::vector<Task> TaskManager::getTasksByPriority(const std::string &priority) const
{
    return FilterUtils::filterByPriority(tasks, priority);
}

std::vector<Task> TaskManager::searchTasksByKeyword(const std::string &keyword) const
{
    return FilterUtils::filterByKeyword(tasks, keyword);
}

std::vector<Task> TaskManager::getTasksDueBy(const std::string &date) const
{
    std::vector<Task> filtered;
    for (const Task &task : tasks)
    {
        if (!task.getDueDate().empty() &&
            (DateUtils::isDateEqual(task.getDueDate(), date) ||
             DateUtils::isDateBefore(task.getDueDate(), date)))
        {
            filtered.push_back(task);
        }
    }
    return filtered;
}

std::vector<Task> TaskManager::getOverdueTasks() const
{
    return FilterUtils::filterOverdue(tasks);
}

std::vector<Task> TaskManager::getTasksDueToday() const
{
    return FilterUtils::filterDueToday(tasks);
}

std::vector<Task> TaskManager::getTasksFiltered(const std::string &keyword,
                                                const std::string &priority,
                                                const std::string &status,
                                                const std::string &due_date,
                                                bool overdue_only,
                                                bool due_today_only) const
{
    return FilterUtils::filterByCriteria(tasks, keyword, priority, status,
                                         due_date, overdue_only, due_today_only);
}

std::vector<Task> TaskManager::getTasksSorted(const std::string &sort_by, bool ascending) const
{
    std::vector<Task> sorted_tasks = tasks;

    if (sort_by == "priority")
    {
        return FilterUtils::sortByPriority(sorted_tasks, ascending);
    }
    else if (sort_by == "due_date" || sort_by == "due")
    {
        return FilterUtils::sortByDueDate(sorted_tasks, ascending);
    }
    else if (sort_by == "status")
    {
        return FilterUtils::sortByStatus(sorted_tasks, ascending);
    }
    else if (sort_by == "id")
    {
        return FilterUtils::sortById(sorted_tasks, ascending);
    }
    else if (sort_by == "created_date" || sort_by == "created")
    {
        return FilterUtils::sortByCreatedDate(sorted_tasks, ascending);
    }

    return sorted_tasks;
}

std::vector<Task> TaskManager::getTasksFilteredAndSorted(const std::string &sort_by,
                                                         bool ascending,
                                                         const std::string &keyword,
                                                         const std::string &priority,
                                                         const std::string &status,
                                                         const std::string &due_date,
                                                         bool overdue_only,
                                                         bool due_today_only) const
{
    return FilterUtils::filterAndSort(tasks, sort_by, ascending, keyword, priority,
                                      status, due_date, overdue_only, due_today_only);
}

void TaskManager::loadTasks()
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        // File doesn't exist yet, start with empty tasks
        return;
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    tasks = JSONParser::parseTasksFromJSON(content);

    // Update nextId to be one higher than the highest existing ID
    for (const Task &task : tasks)
    {
        if (task.getId() >= nextId)
        {
            nextId = task.getId() + 1;
        }
    }
}

void TaskManager::saveTasks() const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Cannot save tasks to file " << filename << std::endl;
        return;
    }

    file << JSONParser::tasksToJSON(tasks);
    file.close();
}

Task *TaskManager::findTaskById(int id)
{
    auto it = std::find_if(tasks.begin(), tasks.end(),
                           [id](const Task &task)
                           { return task.getId() == id; });
    return (it != tasks.end()) ? &(*it) : nullptr;
}

int TaskManager::generateNextId()
{
    return nextId++;
}

size_t TaskManager::getTaskCount() const
{
    return tasks.size();
}
