#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <vector>
#include <string>
#include "task.h"

class TaskManager {
private:
    std::vector<Task> tasks;
    std::string filename;
    int nextId;

public:
    // Constructor
    explicit TaskManager(const std::string& filename = "tasks.json");

    // Basic CRUD operations
    void addTask(const std::string& description);
    void updateTask(int id, const std::string& status);
    void deleteTask(int id);
    
    // Listing operations
    std::vector<Task> getAllTasks() const;
    std::vector<Task> getTasksByStatus(const std::string& status) const;
    
    // File operations
    void loadTasks();
    void saveTasks() const;
    
    // Utility
    Task* findTaskById(int id);
    int generateNextId();
};

#endif