#pragma once
#include <string>
#include <ostream>

enum class Command;

Command command_from_string(std::string const& command);
std::string command_to_string(Command command);
