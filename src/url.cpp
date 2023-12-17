#include "url.h"
#include <regex>
#include <string>

std::string url::parse(std::string const& url, size_t match_index)
{
    std::regex url_pattern(R"(ftp://([^:@]+)(:[^:@]+)?@([^:/]+)(:\d+)?(/[^ ]*)?)");
    std::smatch url_match;
    if (std::regex_search(url, url_match, url_pattern) && url_match.size() > match_index) {
        return url_match[match_index].str();
    }

    return "";
}

std::string url::parse_user(std::string const& url)
{
    std::string parsed = url::parse(url, 1);
    if (parsed == "") {
        return "anonymous";
    }

    return parsed;
}

std::string url::parse_pass(std::string const& url)
{
    std::string parsed = url::parse(url, 2);
    if (parsed[0] == ':' && parsed.length() > 1) {
        return parsed.substr(1);
    }

    return parsed;
}

std::string url::parse_host(std::string const& url) { return url::parse(url, 3); }

std::string url::parse_port(std::string const& url)
{
    std::string parsed = url::parse(url, 4);
    if (parsed == "") {
        return "21";
    }

    return parsed;
}

std::string url::parse_path(std::string const& url) { return url::parse(url, 5); }

bool url::is_url(std::string const& url)
{
    std::regex url_pattern(R"(ftp://([^:@]+)(:[^:@]+)?@([^:/]+)(:\d+)?(/[^ ]*)?)");
    return std::regex_search(url, url_pattern);
}
