#include "EmailMessage.hh"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace sieve
{

std::string EmailMessage::_toLower(const std::string &s) {
    std::string result = s;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

EmailMessage EmailMessage::parse(const std::string &filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Could not open email file: " + filepath);
    }

    EmailMessage msg;
    msg._size = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    std::istringstream stream(content);
    std::string line;
    std::string lastHeaderName;
    bool inHeaders = true;
    std::string bodyAccum;

    while (std::getline(stream, line)) {
        // Strip trailing \r
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (inHeaders) {
            // Blank line ends headers
            if (line.empty()) {
                inHeaders = false;
                continue;
            }

            // Continuation line (starts with whitespace)
            if ((line[0] == ' ' || line[0] == '\t') && !lastHeaderName.empty()) {
                // Unfold: append to previous header value
                auto range = msg._headers.equal_range(lastHeaderName);
                if (range.first != range.second) {
                    auto last = std::prev(range.second);
                    std::string cont = line.substr(line.find_first_not_of(" \t"));
                    auto contEnd = cont.find_last_not_of(" \t");
                    if (contEnd != std::string::npos) cont = cont.substr(0, contEnd + 1);
                    else cont.clear();
                    last->second += " " + cont;
                }
                continue;
            }

            // New header
            auto colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string name = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);

                // Trim leading and trailing whitespace from value (RFC 5228 §5.4)
                auto start = value.find_first_not_of(" \t");
                if (start != std::string::npos) {
                    value = value.substr(start);
                } else {
                    value.clear();
                }
                auto end = value.find_last_not_of(" \t\r");
                if (end != std::string::npos) {
                    value = value.substr(0, end + 1);
                } else {
                    value.clear();
                }

                lastHeaderName = _toLower(name);
                msg._headers.emplace(lastHeaderName, value);
            }
        } else {
            if (!bodyAccum.empty()) {
                bodyAccum += "\n";
            }
            bodyAccum += line;
        }
    }

    msg._body = std::move(bodyAccum);
    return msg;
}

std::vector<std::string> EmailMessage::header(const std::string &name) const {
    std::vector<std::string> values;
    std::string lowerName = _toLower(name);
    auto range = _headers.equal_range(lowerName);
    for (auto it = range.first; it != range.second; ++it) {
        values.push_back(it->second);
    }
    return values;
}

bool EmailMessage::hasHeader(const std::string &name) const {
    return _headers.find(_toLower(name)) != _headers.end();
}

} // namespace sieve
