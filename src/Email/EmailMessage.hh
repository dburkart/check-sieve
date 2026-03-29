#pragma once

#include <map>
#include <string>
#include <vector>

namespace sieve
{

class EmailMessage {
public:
    static EmailMessage parse(const std::string &filepath);

    std::vector<std::string> header(const std::string &name) const;
    bool hasHeader(const std::string &name) const;
    size_t size() const { return _size; }
    const std::string& body() const { return _body; }

private:
    static std::string _toLower(const std::string &s);

    std::multimap<std::string, std::string> _headers;
    std::string _body;
    size_t _size = 0;
};

} // namespace sieve
