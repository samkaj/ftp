#include "request.h"
#include "command.h"
#include <string>
#include <vector>

Request::Request(std::string user_input)
{
    parse_command_and_args(user_input);
}

void Request::parse_command_and_args(std::string user_input)
{
    size_t i = user_input.find(' ');
    if (i == std::string::npos) {
        command = command_from_string(user_input);
        return;
    }
    command = command_from_string(user_input.substr(0, i));
    std::string arg = user_input.substr(i + 1);
    std::vector<std::string> tokens = split(arg, ' ');
    for (auto& token : tokens) {
        if (token.size() > 0) {
            args.push_back(token);
        }
    }
}

std::vector<std::string> Request::split(std::string const& s, char delim)
{
    std::vector<std::string> tokens;
    size_t start { 0 };
    size_t end { 0 };
    while ((end = s.find(delim, start)) != std::string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(s.substr(start));
    return tokens;
}
