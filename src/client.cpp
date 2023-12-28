#include "client.h"
#include "url.h"
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>

Client::Client(std::vector<std::string> args)
{
    std::cout << "Creating client\n";
    operations = {
        { "mkdir", [this]() { this->mkdir(); } },
        { "rmdir", [this]() { this->rmdir(); } },
        { "ls", [this]() { this->ls(); } },
        { "cp", [this]() { this->cp(); } },
        { "rm", [this]() { this->rm(); } }
    };
    parse_user_input(args);
    params = args;
}

void Client::create_socket_and_connect(int& sockfd, std::string const& host, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        throw std::runtime_error("failed to open socket");
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host.c_str());

    std::cout << "Connecting to " << host << ":" << port << "\n";
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        close(sockfd);
        std::cerr << strerror(errno) << "\n";
        throw std::runtime_error("failed to connect to host");
    }
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
        throw std::runtime_error("no url specified");
    }

    std::string url = "";
    std::cout << "Parsing url: " << url << "\n";
    for (auto& arg : args) {
        if (arg == "" || !url::is_url(arg)) {
            local_path = arg;
            continue;
        }
        url = arg;
    }

    if (url == "") {
        throw std::runtime_error("no valid url found");
    }

    host = url::parse_host(url);
    username = url::parse_user(url);
    password = url::parse_pass(url);
    external_path = url::parse_path(url);
    control_port = std::stoi(url::parse_port(url).substr(1));

    if (host == "" || external_path == "") {
        throw std::runtime_error("invalid url, host and path are required");
    }
}

bool Client::receive_response(int sockfd, std::string& response)
{
    char buffer[1024];
    int bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        std::cerr << "Error in receiving data: " << strerror(errno) << "\n";
        throw std::runtime_error("error in receiving data");
    }
    buffer[bytes_received] = '\0';
    response = buffer;
    std::cout << "Response: " << response << "\n";
    return response[0] != '5';
}

void Client::ftp_connect_data_channel()
{
    create_socket_and_connect(data_socket, host, data_port);
}

void Client::send_command()
{
    std::cout << "Sending command\n";
    create_socket_and_connect(control_socket, host, control_port);

    ftp_user();
    ftp_pass();
    ftp_type();
    ftp_mode();
    ftp_stru();

    do_operation(control_socket);

    ftp_quit();
}

void Client::do_operation(int sockfd)
{
    auto it = operations.find(operation);
    if (it != operations.end()) {
        it->second();
    } else {
        std::cerr << "Received unknown operation: " << operation << ", ignoring\n"
                  << std::endl;
    }
}

void Client::ls()
{
    ftp_pasv(); // Send PASV which sets the port for the data channel
    ftp_control_command("LIST " + external_path, "failed to send LIST");
    ftp_connect_data_channel();
    std::string response;
    receive_response(data_socket, response);
    close(data_socket);
}

bool Client::ftp_pasv()
{
    std::string msg = "PASV \r\n";
    std::cout << msg;
    send(control_socket, msg.c_str(), msg.length(), 0);

    std::string response;
    receive_response(control_socket, response);
    std::cout << response << '\n';
    // Make sure the response is indicating further action
    data_port = parse_data_port(response);
    if (data_port == -1) {
        receive_response(control_socket, response);
        data_port = parse_data_port(response);
    }
    std::cout << "port: " << data_port << "\n";
    return response[0] != '5';
}

int Client::parse_data_port(std::string const& input)
{
    // Find the position of the last comma
    auto last_pos = input.rfind(',');
    if (last_pos == std::string::npos) {
        return -1;
    }

    // Extract the number after the last comma
    int last;
    std::stringstream(input.substr(last_pos + 1)) >> last;

    // Find the position of the second to last comma
    auto second_pos = input.rfind(',', last_pos - 1);
    if (second_pos == std::string::npos) {
        return -1;
    }

    // Extract the number before the last comma
    int second;
    std::stringstream(input.substr(second_pos + 1, last_pos - second_pos - 1)) >> second;

    // Perform the calculation
    return (second << 8) + last;
}

void Client::ftp_control_command(std::string const& command, std::string const& error_msg)
{

    std::cout << "Sending " << command << " to FTP server\n";
    std::string msg = command + "\r\n";
    send(control_socket, msg.c_str(), msg.length(), 0);

    char buffer[1024];
    int bytes_received = recv(control_socket, buffer, 1023, 0);

    if (bytes_received < 0) {
        std::cerr << "Error in receiving data: " << strerror(errno) << "\n";
        throw std::runtime_error("error in receiving data");
    }

    buffer[bytes_received] = '\0';

    std::string response(buffer);
    std::cout << "Response: " << response << "\n";
}

void Client::cp()
{
    bool copy_from = url::is_url(params[1]);
    ftp_pasv();
    if (copy_from) {
        ftp_retr(external_path);
        ftp_connect_data_channel();
        cp_from();
    } else {
        ftp_stor(external_path);
        ftp_connect_data_channel();
        cp_to();
    }
}

void Client::cp_to()
{
    std::ifstream file(local_path);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return;
    }

    char buffer[1024];
    while (file.read(buffer, sizeof(buffer) || file.gcount())) {
        int bytes_read = file.gcount();
        int sent = send(data_socket, buffer, bytes_read, 0);
        if (sent < 0) {
            std::cerr << "Failed to send data\n";
            break;
        }
    }

    file.close();
    close(data_socket);
}

void Client::cp_from()
{
    std::cout << "local: " << local_path << "\n";
    std::ofstream file(local_path);
    if (!file) {
        std::cerr << "Failed to open file for writing\n";
        return;
    }

    int ret;
    char buffer[1024];

    while ((ret = read(data_socket, buffer, sizeof(buffer))) > 0) {
        file.write(buffer, ret);
    }

    if (ret < 0) {
        std::cerr << "Error reading from socket\n";
    }

    file.close();
    close(data_socket);
}

void Client::ftp_stor(std::string const& source)
{
    ftp_control_command("STOR " + source, "failed to send STOR");
}

void Client::ftp_retr(std::string const& source)
{
    ftp_control_command("RETR " + source, "failed to send RETR");
}

void Client::rm()
{
    ftp_control_command("DELE " + external_path, "failed to send DELE");
}

void Client::mkdir()
{
    ftp_control_command("MKD " + external_path, "failed to send MKD");
}

void Client::rmdir()
{
    ftp_control_command("RMD " + external_path, "failed to send RMD");
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
    close(data_socket);
    close(control_socket);
}
