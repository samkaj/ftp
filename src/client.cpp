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
    parse_user_input(args);
}

// Build the client object from the user input. Based on the user input,
// the client finds the operation, host, username, password, port, and external path.
void Client::parse_user_input(std::vector<std::string> args)
{
    operation = { args[0] };
    args.erase(args.begin());
    params = args;
    std::string url = url::is_url(params[0]) && params.size() > 0 ? params[0] : params[1];
    if (!url::is_url(url)) {
        throw "invalid url";
    }

    host = url::parse_host(url);
    username = url::parse_user(url);
    password = url::parse_pass(url);
    external_path = url::parse_path(url);
    port = std::stoi(url::parse_port(url));

    if (host == "" || external_path == "") {
        throw "invalid url, host and path are required";
    }
}

void Client::connect_to_server()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw "failed to open socket";
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // FIXME: resolve ip

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sockfd);
        throw "failed to connect to FTP server";
    }

    ftp_user(sockfd);
    ftp_pass(sockfd);
    ftp_type(sockfd);
    ftp_mode(sockfd);
    ftp_stru(sockfd);

    // send operation

    // send EXIT
    ftp_quit(sockfd);
    close(sockfd);
}


void Client::ftp_command(int sockfd, std::string const& command, std::string const& error_msg)
{
    std::string msg = command + "\r\n";
    send(sockfd, msg.c_str(), msg.length(), 0);

    char buffer[1024];
    recv(sockfd, buffer, 1024, 0);
    if (buffer[0] != '2' || buffer[0] != '3') {
        ftp_quit(sockfd);
        throw error_msg;
    }

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
