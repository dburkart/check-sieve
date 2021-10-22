#include <iostream>
#include <sstream>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#include "MailServer.hh"

namespace sieve {

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

std::vector<std::string> MailServer::capabilities()
{
    this->_connect();

    std::istringstream greeting_string(_greeting);
    for (std::string line; std::getline(greeting_string, line);)
    {
        if (line.rfind("\"SIEVE\"", 0) == 0)
        {
            std::cout << line << std::endl;
        }

    }

    return {};
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

} //-- namespace sieve