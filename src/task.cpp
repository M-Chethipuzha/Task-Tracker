#include "task.h"
#include "date_utils.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Task::Task() : id(0), description(""), status("pending"), priority("medium"), due_date(""), created_date("")
{
    created_date = DateUtils::getCurrentDate();
}

Task::Task(int id, const std::string &description, const std::string &priority,
           const std::string &due_date, const std::string &created_date)
    : id(id), description(description), status("pending"), priority(priority), due_date(due_date), created_date(created_date)
{

    if (!isValidPriority(priority))
    {
        this->priority = "medium";
    }

    if (created_date.empty())
    {
        this->created_date = DateUtils::getCurrentDate();
    }

    // Validate due_date if provided
    if (!due_date.empty() && !DateUtils::isValidDate(due_date))
    {
        std::cerr << "Warning: Invalid due date format. Due date cleared." << std::endl;
        this->due_date = "";
    }
}

// Getters
int Task::getId() const { return id; }
std::string Task::getDescription() const { return description; }
std::string Task::getStatus() const { return status; }
std::string Task::getPriority() const { return priority; }
std::string Task::getDueDate() const { return due_date; }
std::string Task::getCreatedDate() const { return created_date; }

// Setters
void Task::setId(int id) { this->id = id; }
void Task::setDescription(const std::string &description) { this->description = description; }

void Task::setStatus(const std::string &status)
{
    if (isValidStatus(status))
    {
        this->status = status;
    }
    else
    {
        std::cerr << "Invalid status: " << status << ". Using 'pending' instead." << std::endl;
        this->status = "pending";
    }
}

void Task::setPriority(const std::string &priority)
{
    if (isValidPriority(priority))
    {
        this->priority = priority;
    }
    else
    {
        std::cerr << "Invalid priority: " << priority << ". Using 'medium' instead." << std::endl;
        this->priority = "medium";
    }
}

void Task::setDueDate(const std::string &due_date)
{
    if (due_date.empty() || DateUtils::isValidDate(due_date))
    {
        this->due_date = due_date;
    }
    else
    {
        std::cerr << "Invalid due date format: " << due_date << ". Expected YYYY-MM-DD." << std::endl;
    }
}

void Task::setCreatedDate(const std::string &created_date)
{
    if (DateUtils::isValidDate(created_date))
    {
        this->created_date = created_date;
    }
}

// Validation methods
bool Task::isValidStatus(const std::string &status) const
{
    return (status == "pending" || status == "in_progress" || status == "done");
}

bool Task::isValidPriority(const std::string &priority) const
{
    return (priority == "high" || priority == "medium" || priority == "low");
}

// Utility methods
bool Task::isDueToday() const
{
    if (due_date.empty())
        return false;
    return DateUtils::isDateEqual(due_date, DateUtils::getCurrentDate());
}

bool Task::isOverdue() const
{
    if (due_date.empty())
        return false;
    return DateUtils::isDateBefore(due_date, DateUtils::getCurrentDate());
}

int Task::getDaysUntilDue() const
{
    if (due_date.empty())
        return -1;
    return DateUtils::daysBetween(DateUtils::getCurrentDate(), due_date);
}

bool Task::matchesKeyword(const std::string &keyword) const
{
    std::string desc_lower = description;
    std::string keyword_lower = keyword;

    std::transform(desc_lower.begin(), desc_lower.end(), desc_lower.begin(), ::tolower);
    std::transform(keyword_lower.begin(), keyword_lower.end(), keyword_lower.begin(), ::tolower);

    return desc_lower.find(keyword_lower) != std::string::npos;
}

std::string Task::toString() const
{
    std::stringstream ss;
    ss << "ID: " << id << ", Description: " << description
       << ", Status: " << status << ", Priority: " << priority;
    if (!due_date.empty())
    {
        ss << ", Due: " << due_date;
    }
    ss << ", Created: " << created_date;
    return ss.str();
}

// Comparison operators
bool Task::operator<(const Task &other) const
{
    return id < other.id;
}

bool Task::operator>(const Task &other) const
{
    return id > other.id;
}

bool Task::operator==(const Task &other) const
{
    return id == other.id;
}