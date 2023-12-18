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

    ftp_user(control_channel_fd);
    ftp_pass(control_channel_fd);
    ftp_type(control_channel_fd);
    ftp_mode(control_channel_fd);
    ftp_stru(control_channel_fd);

    // TODO: send operation

    ftp_quit(control_channel_fd);
    close(control_channel_fd);
}

void Client::ftp_command(int sockfd, std::string const& command, std::string const& error_msg)
{
    std::cout << "Sending " << command << " to FTP server\n";
    std::string msg = command + "\r\n";
    send(sockfd, msg.c_str(), msg.length(), 0);

    char buffer[1024];
    recv(sockfd, buffer, 1024, 0);
    if (buffer[0] != '2' && buffer[0] != '3') {
        ftp_quit(sockfd);
        throw error_msg;
    }

    std::string response(buffer);
    std::cout << "Response: " << response << "\n";
    memset(buffer, 0, sizeof(buffer));
}

void Client::ftp_user(int sockfd)
{
    ftp_command(sockfd, "USER " + username, "failed to send USER");
}

void Client::ftp_pass(int sockfd)
{
    ftp_command(sockfd, "PASS " + password, "failed to send PASS");
}

void Client::ftp_type(int sockfd)
{
    ftp_command(sockfd, "TYPE I", "failed to send TYPE");
}

void Client::ftp_mode(int sockfd)
{
    ftp_command(sockfd, "MODE S", "failed to send MODE");
}

void Client::ftp_stru(int sockfd)
{
    ftp_command(sockfd, "STRU F", "failed to send STRU");
}

void Client::ftp_quit(int sockfd)
{
    ftp_command(sockfd, "QUIT", "failed to send QUIT");
}
