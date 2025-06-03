#include "task.h"
#include <iostream>
#include <sstream>

Task::Task() : id(0), description(""), status("pending") {}

Task::Task(int id, const std::string& description, const std::string& status)
    : id(id), description(description), status(status) {
    if (!isValidStatus(status)) {
        this->status = "pending";
    }
}

int Task::getId() const {
    return id;
}

std::string Task::getDescription() const {
    return description;
}

std::string Task::getStatus() const {
    return status;
}

void Task::setId(int id) {
    this->id = id;
}

void Task::setDescription(const std::string& description) {
    this->description = description;
}

void Task::setStatus(const std::string& status) {
    if (isValidStatus(status)) {
        this->status = status;
    } else {
        std::cerr << "Invalid status: " << status << ". Using 'pending' instead." << std::endl;
        this->status = "pending";
    }
}

bool Task::isValidStatus(const std::string& status) const {
    return (status == "pending" || status == "in_progress" || status == "done");
}

std::string Task::toString() const {
    std::stringstream ss;
    ss << "ID: " << id << ", Description: " << description << ", Status: " << status;
    return ss.str();
}