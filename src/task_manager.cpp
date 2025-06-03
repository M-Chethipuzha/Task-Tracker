#include "task_manager.h"
#include "json_parser.h"
#include <fstream>
#include <iostream>
#include <algorithm>

TaskManager::TaskManager(const std::string& filename) 
    : filename(filename), nextId(1) {
    loadTasks();
}

void TaskManager::addTask(const std::string& description) {
    if (description.empty()) {
        std::cerr << "Error: Task description cannot be empty." << std::endl;
        return;
    }
    
    int id = generateNextId();
    tasks.emplace_back(id, description, "pending");
    saveTasks();
    std::cout << "Task added successfully with ID: " << id << std::endl;
}

void TaskManager::updateTask(int id, const std::string& status) {
    Task* task = findTaskById(id);
    if (task == nullptr) {
        std::cerr << "Error: Task with ID " << id << " not found." << std::endl;
    }
    
    if (!task->isValidStatus(status)) {
        std::cerr << "Error: Invalid status '" << status << "'. Valid statuses are: pending, in_progress, done" << std::endl;
    }
    
    task->setStatus(status);
    saveTasks();
    std::cout << "Task " << id << " updated successfully." << std::endl;
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