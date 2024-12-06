#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <arpa/inet.h>
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

std::map<std::string_view, bool> MailServer::capabilities()
{
    auto capabilities = std::map<std::string_view, bool>();

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

    return capabilities;
}

void MailServer::_connect()
{
    if (_socket >= 0)
        return;

    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        abort();
    }

    struct sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(this->_port);

    struct hostent *host_entry;
    struct in_addr **address_list;

    if ((host_entry = gethostbyname(this->_hostname.c_str())) == nullptr) {
        herror("gethostbyname");
        abort();
    }

    address_list = (struct in_addr **) host_entry->h_addr_list;

    char ip[100] = {};
    if (address_list[0] == nullptr)
    {
        std::cout << "Could not map hostname: " << this->_hostname << " to an IP address." << std::endl;
        abort();
    }
    strcpy(ip, inet_ntoa(*address_list[0]));

    if (inet_pton(AF_INET, ip, &server_address.sin_addr) <= 0)
    {
        std::cout << "Invalid address / Address not supported" << ip << std::endl;
        abort();
    }

    if (connect(_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
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