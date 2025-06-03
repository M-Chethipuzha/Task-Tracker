#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <string>
#include "task.h"

class TaskManager
{
private:
    std::vector<Task> tasks;
    std::string filename;
    int nextId;

public:
    // Constructor
    explicit TaskManager(const std::string &filename = "tasks.json");

    // Enhanced CRUD operations
    void addTask(const std::string &description, const std::string &priority = "medium",
                 const std::string &due_date = "");
    bool updateTask(int id, const std::string &description = "",
                    const std::string &status = "", const std::string &priority = "",
                    const std::string &due_date = "");
    bool deleteTask(int id);

    // Basic listing operations
    std::vector<Task> getAllTasks() const;
    std::vector<Task> getTasksByStatus(const std::string &status) const;
    std::vector<Task> getTasksByPriority(const std::string &priority) const;
    std::vector<Task> searchTasksByKeyword(const std::string &keyword) const;
    std::vector<Task> getTasksDueBy(const std::string &date) const;
    std::vector<Task> getOverdueTasks() const;
    std::vector<Task> getTasksDueToday() const;

    // Enhanced filtering and sorting (integrating FilterUtils)
    std::vector<Task> getTasksFiltered(const std::string &keyword = "",
                                       const std::string &priority = "",
                                       const std::string &status = "",
                                       const std::string &due_date = "",
                                       bool overdue_only = false,
                                       bool due_today_only = false) const;

    std::vector<Task> getTasksSorted(const std::string &sort_by = "id",
                                     bool ascending = true) const;

    std::vector<Task> getTasksFilteredAndSorted(const std::string &sort_by = "",
                                                bool ascending = true,
                                                const std::string &keyword = "",
                                                const std::string &priority = "",
                                                const std::string &status = "",
                                                const std::string &due_date = "",
                                                bool overdue_only = false,
                                                bool due_today_only = false) const;

    // File operations
    void loadTasks();
    void saveTasks() const;

    // Utility
    Task *findTaskById(int id);
    int generateNextId();
    size_t getTaskCount() const;
};

#endif