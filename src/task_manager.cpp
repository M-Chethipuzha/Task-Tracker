#include "task_manager.h"
#include "json_parser.h"
#include "date_utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>

TaskManager::TaskManager(const std::string& filename) 
    : filename(filename), nextId(1) {
    loadTasks();
}

void TaskManager::addTask(const std::string& description, const std::string& priority, 
                         const std::string& due_date) {
    if (description.empty()) {
        std::cerr << "Error: Task description cannot be empty." << std::endl;
        return;
    }
    
    int id = generateNextId();
    Task task(id, description, priority, due_date);
    tasks.push_back(task);
    saveTasks();
    
    std::cout << "Task added successfully with ID: " << id;
    if (!priority.empty() && priority != "medium") {
        std::cout << " (Priority: " << priority << ")";
    }
    if (!due_date.empty()) {
        std::cout << " (Due: " << due_date << ")";
    }
    std::cout << std::endl;
}

void TaskManager::updateTask(int id, const std::string& description, 
                           const std::string& status, const std::string& priority, 
                           const std::string& due_date) {
    Task* task = findTaskById(id);
    if (task == nullptr) {
        std::cerr << "Error: Task with ID " << id << " not found." << std::endl;
    }
    
    bool updated = false;
    
    if (!description.empty()) {
        task->setDescription(description);
        updated = true;
    }
    
    if (!status.empty()) {
        if (!task->isValidStatus(status)) {
            std::cerr << "Error: Invalid status '" << status << "'. Valid statuses are: pending, in_progress, done" << std::endl;
        }
        task->setStatus(status);
        updated = true;
    }
    
    if (!priority.empty()) {
        if (!task->isValidPriority(priority)) {
            std::cerr << "Error: Invalid priority '" << priority << "'. Valid priorities are: high, medium, low" << std::endl;
        }
        task->setPriority(priority);
        updated = true;
    }
    
    if (!due_date.empty()) {
        if (!DateUtils::isValidDate(due_date)) {
            std::cerr << "Error: Invalid date format '" << due_date << "'. Please use YYYY-MM-DD format." << std::endl;
        }
        task->setDueDate(due_date);
        updated = true;
    }
    
    if (updated) {
        saveTasks();
        std::cout << "Task " << id << " updated successfully." << std::endl;
    }
    

}

void TaskManager::deleteTask(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                          [id](const Task& task) { return task.getId() == id; });
    
    if (it == tasks.end()) {
        std::cerr << "Error: Task with ID " << id << " not found." << std::endl;
    }
    
    tasks.erase(it);
    saveTasks();
    std::cout << "Task " << id << " deleted successfully." << std::endl;
}

std::vector<Task> TaskManager::getAllTasks() const {
    return tasks;
}

std::vector<Task> TaskManager::getTasksByStatus(const std::string& status) const {
    std::vector<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (task.getStatus() == status) {
            filteredTasks.push_back(task);
        }
    }
    return filteredTasks;
}

std::vector<Task> TaskManager::getTasksByPriority(const std::string& priority) const {
    std::vector<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (task.getPriority() == priority) {
            filteredTasks.push_back(task);
        }
    }
    return filteredTasks;
}

std::vector<Task> TaskManager::searchTasksByKeyword(const std::string& keyword) const {
    std::vector<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (task.matchesKeyword(keyword)) {
            filteredTasks.push_back(task);
        }
    }
    return filteredTasks;
}

std::vector<Task> TaskManager::getTasksDueBy(const std::string& date) const {
    std::vector<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (!task.getDueDate().empty() && 
            (DateUtils::isDateEqual(task.getDueDate(), date) || 
             DateUtils::isDateBefore(task.getDueDate(), date))) {
            filteredTasks.push_back(task);
        }
    }
    return filteredTasks;
}

std::vector<Task> TaskManager::getOverdueTasks() const {
    std::vector<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (task.isOverdue()) {
            filteredTasks.push_back(task);
        }
    }
    return filteredTasks;
}

std::vector<Task> TaskManager::getTasksDueToday() const {
    std::vector<Task> filteredTasks;
    for (const Task& task : tasks) {
        if (task.isDueToday()) {
            filteredTasks.push_back(task);
        }
    }
    return filteredTasks;
}

void TaskManager::loadTasks() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // File doesn't exist yet, start with empty tasks
        return;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    
    tasks = JSONParser::parseTasksFromJSON(content);
    
    // Update nextId to be one higher than the highest existing ID
    for (const Task& task : tasks) {
        if (task.getId() >= nextId) {
            nextId = task.getId() + 1;
        }
    }
}

void TaskManager::saveTasks() const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot save tasks to file " << filename << std::endl;
        return;
    }
    
    file << JSONParser::tasksToJSON(tasks);
    file.close();
}

Task* TaskManager::findTaskById(int id) {
    auto it = std::find_if(tasks.begin(), tasks.end(),
                          [id](const Task& task) { return task.getId() == id; });
    return (it != tasks.end()) ? &(*it) : nullptr;
}

int TaskManager::generateNextId() {
    return nextId++;
}