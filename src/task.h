#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
private:
    int id;
    std::string description;
    std::string status; // "pending", "in_progress", "done"

public:
    // Constructors
    Task();
    Task(int id, const std::string& description, const std::string& status = "pending");

    // Getters
    int getId() const;
    std::string getDescription() const;
    std::string getStatus() const;

    // Setters
    void setId(int id);
    void setDescription(const std::string& description);
    void setStatus(const std::string& status);

    // Utility methods
    bool isValidStatus(const std::string& status) const;
    std::string toString() const;
};

#endif