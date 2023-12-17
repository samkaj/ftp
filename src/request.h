#pragma once
#include "command.h"
#include <string>
#include <vector>

class Request {
public:
    Command command;
    std::vector<std::string> args;
    Request(std::string const& user_input);

private:
    void parse_command_and_args(std::string const& user_input);
    std::vector<std::string> split(std::string const& s, char delim);
};
