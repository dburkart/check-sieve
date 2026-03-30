#pragma once

#include <map>
#include <string>
#include <vector>

namespace sieve
{

struct MimePart {
    std::string contentType;   // normalized lowercase, e.g. "text/plain"
    std::string body;          // body text for this part
    bool isMultipart = false;  // true for multipart prologue/epilogue entries
};

class EmailMessage {
public:
    static EmailMessage parse(const std::string &filepath);

    std::vector<std::string> header(const std::string &name) const;
    bool hasHeader(const std::string &name) const;
    size_t size() const { return _size; }
    const std::string& body() const { return _body; }
    std::vector<MimePart> mimeParts() const;

private:
    static std::string _toLower(const std::string &s);
    static std::string _parseContentType(const std::string &ctHeader);
    static std::string _parseBoundary(const std::string &ctHeader);
    static std::vector<MimePart> _parseMimeParts(
        const std::multimap<std::string, std::string> &headers,
        const std::string &body);

    std::multimap<std::string, std::string> _headers;
    std::string _body;
    size_t _size = 0;
};

} // namespace sieve
