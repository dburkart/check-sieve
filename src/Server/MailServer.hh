#ifndef __MAILSERVER_H__
#define __MAILSERVER_H__

#include <string>
#include <vector>

namespace sieve{

class MailServer {
public:
    MailServer(std::string hostname, uint32_t port);
    ~MailServer();


    std::vector<std::string> capabilities();

private:
    void _connect();

    std::string _hostname;
    uint32_t _port;
    int _socket;

    std::string _greeting;
};

}

#endif //__MAILSERVER_H__
