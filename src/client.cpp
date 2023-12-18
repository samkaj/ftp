#include "client.h"
#include "url.h"
#include <string>
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

void Client::ls(std::string const& external_path) { }
void Client::mkdir(std::string const& external_path) { }
void Client::rm(std::string const& external_filepath) { }
void Client::rmdir(std::string const& external_path) { }
void Client::cp(std::string const& filepath, std::string const& external_path) { }
void Client::mv(std::string const& filepath, std::string const& external_path) { }
