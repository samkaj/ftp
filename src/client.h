#pragma once
#include <string>
#include <vector>

class Client {
public:
    Client(std::vector<std::string> args);
    void send_command();

private:
    int control_socket;
    int port;
    std::string host;
    std::string operation;
    std::string username;
    std::string password;
    std::string external_path;
    std::vector<std::string> params;
    void parse_user_input(std::vector<std::string> args);
    void ls();
    void ftp_user(int sockfd);
    void ftp_pass(int sockfd);
    void ftp_type(int sockfd);
    void ftp_mode(int sockfd);
    void ftp_stru(int sockfd);
    void ftp_quit(int sockfd);
    void ftp_command(int sockfd, std::string const& command, std::string const& error_msg);
};
