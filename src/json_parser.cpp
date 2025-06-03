#include "json_parser.h"
#include <sstream>
#include <iostream>

namespace JSONParser {

std::string escapeJSON(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        switch (c) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += c; break;
        }
    }
    return escaped;
}

std::string unescapeJSON(const std::string& str) {
    std::string unescaped;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case '"': unescaped += '"'; i++; break;
                case '\\': unescaped += '\\'; i++; break;
                case 'n': unescaped += '\n'; i++; break;
                case 'r': unescaped += '\r'; i++; break;
                case 't': unescaped += '\t'; i++; break;
                default: unescaped += str[i]; break;
            }
        } else {
            unescaped += str[i];
        }
    }
    return unescaped;
}

std::string extractStringField(const std::string& objectStr, const std::string& fieldName) {
    std::string searchStr = "\"" + fieldName + "\":";
    size_t fieldPos = objectStr.find(searchStr);
    if (fieldPos == std::string::npos) return "";
    
    size_t quoteStart = objectStr.find('"', fieldPos + searchStr.length());
    if (quoteStart == std::string::npos) return "";
    
    size_t quoteEnd = objectStr.find('"', quoteStart + 1);
    if (quoteEnd == std::string::npos) return "";
    
    return unescapeJSON(objectStr.substr(quoteStart + 1, quoteEnd - quoteStart - 1));
}

int extractIntField(const std::string& objectStr, const std::string& fieldName) {
    std::string searchStr = "\"" + fieldName + "\":";
    size_t fieldPos = objectStr.find(searchStr);
    if (fieldPos == std::string::npos) return 0;
    
    size_t valueStart = objectStr.find_first_not_of(" \t", fieldPos + searchStr.length());
    if (valueStart == std::string::npos) return 0;
    
    size_t valueEnd = objectStr.find_first_of(",}", valueStart);
    if (valueEnd == std::string::npos) return 0;
    
    try {
        return std::stoi(objectStr.substr(valueStart, valueEnd - valueStart));
    } catch (const std::exception&) {
        return 0;
    }
}

std::vector<Task> parseTasksFromJSON(const std::string& jsonContent) {
    std::vector<Task> tasks;
    
    if (jsonContent.empty() || jsonContent == "[]") {
        return tasks;
    }

    size_t pos = 0;
    
    // Find the opening bracket
    pos = jsonContent.find('[');
    if (pos == std::string::npos) {
        std::cerr << "Invalid JSON: No opening bracket found" << std::endl;
        return tasks;
    }
    pos++;

    while (pos < jsonContent.length()) {
        // Skip whitespace
        while (pos < jsonContent.length() && std::isspace(jsonContent[pos])) {
            pos++;
        }

        // Check for end of array
        if (pos >= jsonContent.length() || jsonContent[pos] == ']') {
            break;
        }

        // Find opening brace of object
        if (jsonContent[pos] != '{') {
            std::cerr << "Invalid JSON: Expected '{' at position " << pos << std::endl;
            break;
        }
        
        size_t objStart = pos;
        size_t objEnd = jsonContent.find('}', objStart);
        if (objEnd == std::string::npos) {
            std::cerr << "Invalid JSON: No closing brace found" << std::endl;
            break;
        }

        std::string objectStr = jsonContent.substr(objStart + 1, objEnd - objStart - 1);
        
        // Parse the object
        int id = extractIntField(objectStr, "id");
        std::string description = extractStringField(objectStr, "description");
        std::string status = extractStringField(objectStr, "status");
        std::string priority = extractStringField(objectStr, "priority");
        std::string due_date = extractStringField(objectStr, "due_date");
        std::string created_date = extractStringField(objectStr, "created_date");

        if (id > 0 && !description.empty()) {
            Task task(id, description, priority.empty() ? "medium" : priority, due_date, created_date);
            if (!status.empty()) {
                task.setStatus(status);
            }
            tasks.push_back(task);
        }

        pos = objEnd + 1;
        
        // Skip comma if present
        while (pos < jsonContent.length() && (std::isspace(jsonContent[pos]) || jsonContent[pos] == ',')) {
            pos++;
        }
    }

    return tasks;
}

std::string tasksToJSON(const std::vector<Task>& tasks) {
    std::stringstream ss;
    ss << "[\n";
    
    for (size_t i = 0; i < tasks.size(); ++i) {
        const Task& task = tasks[i];
        ss << "  {\n";
        ss << "    \"id\": " << task.getId() << ",\n";
        ss << "    \"description\": \"" << escapeJSON(task.getDescription()) << "\",\n";
        ss << "    \"status\": \"" << task.getStatus() << "\",\n";
        ss << "    \"priority\": \"" << task.getPriority() << "\",\n";
        ss << "    \"due_date\": \"" << task.getDueDate() << "\",\n";
        ss << "    \"created_date\": \"" << task.getCreatedDate() << "\"\n";
        ss << "  }";
        
        if (i < tasks.size() - 1) {
            ss << ",";
        }
        ss << "\n";
    }
    
    ss << "]";
    return ss.str();
}

}