#ifndef __MAILSERVER_H__
#define __MAILSERVER_H__

#include <string>
#include <map>
#include <vector>

namespace sieve{

class MailServer {
public:
    static MailServer create(std::string host_with_port);
    ~MailServer();

    std::map<std::string, bool> capabilities();

protected:
    MailServer(std::string hostname, uint32_t port);

private:
    void _connect();
    std::map<std::string, std::string> _parse_response(std::string response);

    std::string _hostname;
    uint32_t _port;
    int _socket;

    std::string _greeting;
};

}

#endif //__MAILSERVER_H__
