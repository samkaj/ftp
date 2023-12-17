#pragma once
#include <string>
#include <vector>

std::vector<std::string> split(std::string const& s, char delim);
class Client {
public:
    Client(std::string const& user_input);
    void ls(std::string const& external_path);
    void mkdir(std::string const& external_path);
    void rm(std::string const& external_filepath);
    void rmdir(std::string const& external_path);
    void cp(std::string const& filepath, std::string const& external_path);
    void mv(std::string const& filepath, std::string const& external_path);

private:
    int control_socket;
    int port;
    std::string host;
    std::string operation;
    std::string username;
    std::string password;
    std::string external_path;
    std::vector<std::string> params;
    void parse_user_input(std::string const& user_input);
    void connect_to_server();
    void user(std::string const& username);
    void pass(std::string const& password);
    void type(std::string const& type);
    void mode(std::string const& mode);
    void stru(std::string const& stru);
    void quit();
};
