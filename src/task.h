#ifndef TASK_H
#define TASK_H

#include <string>

class Task {
private:
    int id;
    std::string description;
    std::string status;       // "pending", "in_progress", "done"
    std::string priority;     // "high", "medium", "low"
    std::string due_date;     // ISO format: "YYYY-MM-DD" (optional)
    std::string created_date; // ISO format: "YYYY-MM-DD"

public:
    // Constructors
    Task();
    Task(int id, const std::string& description, const std::string& priority = "medium", 
         const std::string& due_date = "", const std::string& created_date = "");

    // Getters
    int getId() const;
    std::string getDescription() const;
    std::string getStatus() const;
    std::string getPriority() const;
    std::string getDueDate() const;
    std::string getCreatedDate() const;

    // Setters
    void setId(int id);
    void setDescription(const std::string& description);
    void setStatus(const std::string& status);
    void setPriority(const std::string& priority);
    void setDueDate(const std::string& due_date);
    void setCreatedDate(const std::string& created_date);

    // Validation methods
    bool isValidStatus(const std::string& status) const;
    bool isValidPriority(const std::string& priority) const;

    // Utility methods
    bool isDueToday() const;
    bool isOverdue() const;
    int getDaysUntilDue() const;
    bool matchesKeyword(const std::string& keyword) const;
    std::string toString() const;
    
    // Comparison operators for sorting
    bool operator<(const Task& other) const;
    bool operator>(const Task& other) const;
    bool operator==(const Task& other) const;
};

#endif