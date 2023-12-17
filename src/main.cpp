#include "command.h"
#include "request.h"
#include <iostream>
#include <string>

void read_commands()
{
    std::string line{};
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            std::cout << "> ";
            continue;
        }
        Request request(line);
        std::cout << command_to_string(request.command) << std::endl;
        std::cout << "> ";
    }
}

int main()
{
    std::cout << "> ";
    read_commands();
    return 0;
}
