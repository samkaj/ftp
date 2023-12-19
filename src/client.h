#pragma once
#include <functional>
#include <map>
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
    std::map<std::string, std::function<void()>> operations;
    void parse_user_input(std::vector<std::string> args);
    void do_operation(int sockfd);
    void mkdir();
    void rmdir();
    void ftp_user();
    void ftp_pass();
    void ftp_type();
    void ftp_mode();
    void ftp_stru();
    void ftp_quit();
    void ftp_control_command(std::string const& command, std::string const& error_msg);
};
