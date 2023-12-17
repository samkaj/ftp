#pragma once
#include <cstddef>
#include <string>

namespace url {
bool is_url(std::string const& url);
std::string parse_host(std::string const& url);
std::string parse_scheme(std::string const& url);
std::string parse_user(std::string const& url);
std::string parse_pass(std::string const& url);
std::string parse_path(std::string const& url);
std::string parse_port(std::string const& url);
std::string parse(std::string const &url, size_t match_index);
}
