#include <iostream>

void read_stdin()
{
    std::string line;
    while (std::getline(std::cin, line)) {
        std::cout << line << std::endl;
        std::cout << "> ";
    }
}

int main()
{
    std::cout << "> ";
    read_stdin();
    return 0;
}
