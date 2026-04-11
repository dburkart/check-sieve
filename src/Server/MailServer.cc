#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <cctype>
#include <netdb.h>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>

#ifdef __FreeBSD__
#include <netinet/in.h>
#endif

#include "MailServer.hh"

namespace sieve {

MailServer MailServer::create(const std::string& host_with_port)
{
    bool in_port = false;
    std::string hostname, port;

    for (char const &c: host_with_port)
    {
        if (c == ':')
        {
            in_port = true;
            continue;
        }

        if (in_port)
            port += c;
        else
            hostname += c;
    }

    return {hostname, static_cast<uint32_t>(atoi(port.c_str()))};
}

MailServer::MailServer(std::string hostname, uint32_t port)
    : _hostname(std::move(hostname))
    , _port(port)
    , _socket(-1)
{
}

MailServer::~MailServer()
{
    close(_socket);
}

std::map<std::string, bool> MailServer::capabilities()
{
    auto capabilities = std::map<std::string, bool>();

    this->_connect();

    auto hello_dictionary = sieve::MailServer::_parse_response(_greeting);

    std::string capability;
    for (char const &c: hello_dictionary["sieve"])
    {
        if (c == ' ')
        {
            capabilities[capability] = true;
            capability = "";
            continue;
        }

        capability += c;
    }

    if (!capability.empty())
        capabilities[capability] = true;

    return capabilities;
}

void MailServer::_connect()
{
    if (_socket >= 0)
        return;

    struct addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string port_str = std::to_string(this->_port);
    struct addrinfo *res;
    int err = getaddrinfo(this->_hostname.c_str(), port_str.c_str(), &hints, &res);
    if (err != 0) {
        std::cout << "getaddrinfo: " << gai_strerror(err) << std::endl;
        abort();
    }

    bool connected = false;
    for (struct addrinfo *ai = res; ai != nullptr; ai = ai->ai_next) {
        _socket = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (_socket < 0)
            continue;
        if (connect(_socket, ai->ai_addr, ai->ai_addrlen) == 0) {
            connected = true;
            break;
        }
        close(_socket);
        _socket = -1;
    }
    freeaddrinfo(res);

    if (!connected) {
        std::cout << "Connection Failed" << std::endl;
        abort();
    }

    char buffer[1024] = {};
    if (send(_socket, "", 0, 0) == -1) {
        std::cout << "Could not send to socket: " << strerror(errno) << std::endl;
        abort();
    }

    if (read(_socket, buffer, 1024) == -1) {
        std::cout << "Could not read from socket: " << strerror(errno) << std::endl;
        abort();
    }

    _greeting = std::string(buffer);
}

std::map<std::string, std::string> MailServer::_parse_response(const std::string& response)
{
    auto dictionary = std::map<std::string, std::string>();

    std::istringstream response_stream(response);
    for (std::string line; std::getline(response_stream, line);)
    {
        bool in_key = false, in_value = false;
        bool key_found = false, value_found = false;
        std::string key, value;
        for (char const &c: line)
        {
            if (c == '"')
            {
                if (!in_key && !key_found) {
                    in_key = true;
                    continue;
                }

                if (in_key) {
                    in_key = false;
                    key_found = true;
                    continue;
                }

                if (!in_value && !value_found) {
                    in_value = true;
                    continue;
                }

                if (in_value) {
                    in_value = false;
                    value_found = true;
                    continue;
                }
            } else {
                if (in_key)
                    key += (char)tolower(c);

                if (in_value)
                    value += c;
            }
        }

        dictionary[key] = value;
    }

    return dictionary;
}


} //-- namespace sieve
