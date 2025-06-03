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

std::vector<Task> parseTasksFromJSON(const std::string& jsonContent) {
    std::vector<Task> tasks;
    
    if (jsonContent.empty() || jsonContent == "[]") {
        return tasks;
    }

    // Simple JSON parser for our specific format
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
        int id = 0;
        std::string description, status;
        
        // Simple field extraction
        size_t idPos = objectStr.find("\"id\":");
        if (idPos != std::string::npos) {
            size_t valueStart = objectStr.find_first_not_of(" \t", idPos + 5);
            size_t valueEnd = objectStr.find_first_of(",}", valueStart);
            if (valueStart != std::string::npos && valueEnd != std::string::npos) {
                id = std::stoi(objectStr.substr(valueStart, valueEnd - valueStart));
            }
        }

        size_t descPos = objectStr.find("\"description\":");
        if (descPos != std::string::npos) {
            size_t quoteStart = objectStr.find('"', descPos + 14);
            size_t quoteEnd = objectStr.find('"', quoteStart + 1);
            if (quoteStart != std::string::npos && quoteEnd != std::string::npos) {
                description = unescapeJSON(objectStr.substr(quoteStart + 1, quoteEnd - quoteStart - 1));
            }
        }

        size_t statusPos = objectStr.find("\"status\":");
        if (statusPos != std::string::npos) {
            size_t quoteStart = objectStr.find('"', statusPos + 9);
            size_t quoteEnd = objectStr.find('"', quoteStart + 1);
            if (quoteStart != std::string::npos && quoteEnd != std::string::npos) {
                status = objectStr.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
            }
        }

        if (id > 0 && !description.empty()) {
            tasks.emplace_back(id, description, status);
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
        ss << "    \"status\": \"" << task.getStatus() << "\"\n";
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