#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#ifdef __FreeBSD__
#include <netinet/in.h>
#endif

#include "MailServer.hh"

namespace sieve {

MailServer MailServer::create(std::string host_with_port)
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

    return MailServer(hostname, atoi(port.c_str()));
}

MailServer::MailServer(std::string hostname, uint32_t port)
    : _hostname(hostname)
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

    auto hello_dictionary = this->_parse_response(_greeting);

    std::string capability = "";
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

    struct sockaddr_in server_address;
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
    send(_socket, "", 0, 0);
    read(_socket, buffer, 1024);

    _greeting = std::string(buffer);
}

std::map<std::string, std::string> MailServer::_parse_response(std::string response)
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
            switch (c) {
                case '"':
                    if (!in_key && !key_found) {
                        in_key = true;
                        break;
                    }

                    if (in_key) {
                        in_key = false;
                        key_found = true;
                        break;
                    }

                    if (!in_value && !value_found) {
                        in_value = true;
                        break;
                    }

                    if (in_value) {
                        in_value = false;
                        value_found = true;
                        break;
                    }
                    break;
                default:
                    if (in_key)
                        key = key + (char)tolower(c);

                    if (in_value)
                        value = value + c;

                    break;
            }
        }

        dictionary[key] = value;
    }

    return dictionary;
}


} //-- namespace sieve