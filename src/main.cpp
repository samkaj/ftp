#include "client.h"
#include <iostream>
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
    if (argc < 2) {
        usage();
        return 1;
    }

    // Build string representation of provided args.
    std::string user_input;
    for (int i = 1; i < argc; ++i) {
        user_input += argv[i];
        if (i < argc - 1) {
            user_input += ' ';
        }
    }

    // Check for help flag.
    if (user_input == "-h" || user_input == "--help") {
        usage();
        return 0;
    }

    // Execute command and catch any errors.
    try {
        Client client { user_input };
    } catch (char const* msg) {
        std::cerr << "Failed to execute command: " << msg << "\n\n";
        usage();
    }
    return 0;
}
