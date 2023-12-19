#include "client.h"
#include "url.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

Client::Client(std::vector<std::string> args)
{
    std::cout << "Creating client\n";
    parse_user_input(args);
}

// Build the client object from the user input. Based on the user input,
// the client finds the operation, host, username, password, port, and external path.
void Client::parse_user_input(std::vector<std::string> args)
{
    if (args.empty()) {
        throw std::runtime_error("no operation specified");
    }

    operation = args[0];
    args.erase(args.begin());
    if (args.empty()) {
        throw "no url specified";
    }

    std::string url = args[0];
    std::cout << "Parsing url: " << url << "\n";
    if (url == "" || !url::is_url(url)) {
        std::cerr << "invalid url: \"" << url << "\"\n";
        throw "invalid url";
    }

    host = url::parse_host(url);
    username = url::parse_user(url);
    password = url::parse_pass(url);
    external_path = url::parse_path(url);
    port = std::stoi(url::parse_port(url).substr(1));

    if (host == "" || external_path == "") {
        throw "invalid url, host and path are required";
    }
}

void Client::send_command()
{
    std::cout << "Sending command\n";
    int control_channel_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (control_channel_fd < 0) {
        throw "failed to open socket";
    }

    control_socket = control_channel_fd;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());

    std::cout << "Connecting to " << host << ":" << port << "\n";
    if (connect(control_channel_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(control_channel_fd);
        std::cerr << strerror(errno) << "\n";
        throw "failed to connect to server";
    }

    ftp_user();
    ftp_pass();
    ftp_type();
    ftp_mode();
    ftp_stru();

    do_operation(control_channel_fd);

    ftp_quit();
    close(control_channel_fd);
}

void Client::do_operation(int sockfd)
{
    if (operation == "mkdir") { }
}

void Client::ftp_control_command(std::string const& command, std::string const& error_msg)
{
    std::cout << "Sending " << command << " to FTP server\n";
    std::string msg = command + "\r\n";
    send(control_socket, msg.c_str(), msg.length(), 0);

    char buffer[1024];
    recv(control_socket, buffer, 1024, 0);
    if (buffer[0] != '2' && buffer[0] != '3') {
        ftp_quit();
        throw error_msg;
    }

    std::string response(buffer);
    std::cout << "Response: " << response << "\n";
    memset(buffer, 0, sizeof(buffer));
}

void Client::ftp_user()
{
    ftp_control_command("USER " + username, "failed to send USER");
}

void Client::ftp_pass()
{
    ftp_control_command("PASS " + password, "failed to send PASS");
}

void Client::ftp_type()
{
    ftp_control_command("TYPE I", "failed to send TYPE");
}

void Client::ftp_mode()
{
    ftp_control_command("MODE S", "failed to send MODE");
}

void Client::ftp_stru()
{
    ftp_control_command("STRU F", "failed to send STRU");
}

void Client::ftp_quit()
{
    ftp_control_command("QUIT", "failed to send QUIT");
}
