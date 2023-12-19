#include "client.h"
#include <iostream>
#include <stdexcept>
#include <string>

// Print usage information.
void usage()
{
    std::cout << "Usage: ftp <command> params [params ...]\n\n";
    std::cout << "List of commands:\n\n";
    std::cout << "ls <url>        \tPrint directory contents from the FTP server\n";
    std::cout << "mkdir <url>     \tCreate a new directory at the given path on the FTP server\n";
    std::cout << "rm <url>        \tDelete the file at the given path specified in the FTP url\n";
    std::cout << "rmdir <url>     \tDelete the directory at the given path specified in the FTP url\n";
    std::cout << "cp <arg1> <arg2>\tCopy the file to either the client or the server\n";
    std::cout << "mv <arg1> <arg2>\tMove the file to either the client or the server\n";
}

// Parse args and call the appropriate function, then exit.
int main(int argc, char* argv[])
{
    // Check for args.
    if (argc < 3) {
        std::cerr << "Error: too few arguments\n\n";
        usage();
        return 1;
    }
    // Execute command and catch any errors.
    std::vector<std::string> args { argv + 1, argv + argc };
    try {
        Client client { args };
        client.send_command();
    } catch (std::runtime_error error) {
        std::cerr << "Failed to execute command: " << error.what() << "\n\n";
        // usage();
    }
    return 0;
}
