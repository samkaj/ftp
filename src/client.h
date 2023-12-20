#pragma once
#include <functional>
#include <map>
#include <string>
#include <vector>

class Client {
public:
    Client(std::vector<std::string> args);
    void send_command();
    int data_port;
    int control_port;

private:
    int control_socket;
    int data_socket;
    std::string host;
    std::string operation;
    std::string username;
    std::string password;
    std::string external_path;
    std::vector<std::string> params;
    std::map<std::string, std::function<void()>> operations;
    void parse_user_input(std::vector<std::string> args);
    int parse_data_port(std::string const& input);
    void do_operation(int sockfd);
    void mkdir();
    void ls();
    void rmdir();
    void ftp_pasv();
    void ftp_user();
    void ftp_pass();
    void ftp_type();
    void ftp_mode();
    void ftp_stru();
    void ftp_quit();
    void create_socket_and_connect(int& sockfd, std::string const& host, int port);
    void ftp_control_command(std::string const& command, std::string const& error_msg);
    void ftp_command(int sockfd, std::string const& command, std::string const& error_msg);
    void ftp_connect_data_channel();
    void receive_response(int sockfd, std::string& response);
};
