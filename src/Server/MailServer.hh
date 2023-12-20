#pragma once

#include <string>
#include <map>
#include <vector>

namespace sieve{

class MailServer {
public:
    static MailServer create(const std::string& host_with_port);
    ~MailServer();

    std::map<std::string_view, bool> capabilities();

protected:
    MailServer(std::string hostname, uint32_t port);

private:
    void _connect();
    static std::map<std::string, std::string> _parse_response(const std::string& response);

    std::string _hostname;
    uint32_t _port;
    int _socket;

    std::string _greeting;
};

}

