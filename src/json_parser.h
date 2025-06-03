#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>
#include "task.h"

namespace JSONParser
{
    std::vector<Task> parseTasksFromJSON(const std::string &jsonContent);
    std::string tasksToJSON(const std::vector<Task> &tasks);
    std::string escapeJSON(const std::string &str);
    std::string unescapeJSON(const std::string &str);
}

#endif