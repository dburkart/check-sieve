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

std::vector<MimePart> EmailMessage::mimeParts() const {
    return _parseMimeParts(_headers, _body);
}

std::string EmailMessage::_parseContentType(const std::string &ctHeader) {
    auto semicolon = ctHeader.find(';');
    std::string type = (semicolon != std::string::npos) ? ctHeader.substr(0, semicolon) : ctHeader;
    // Trim whitespace
    auto start = type.find_first_not_of(" \t");
    auto end = type.find_last_not_of(" \t");
    if (start == std::string::npos) return "";
    type = type.substr(start, end - start + 1);
    return _toLower(type);
}

std::string EmailMessage::_parseBoundary(const std::string &ctHeader) {
    // Find boundary= (case-insensitive)
    std::string lower = _toLower(ctHeader);
    auto pos = lower.find("boundary=");
    if (pos == std::string::npos) return "";
    pos += 9; // skip "boundary="
    if (pos >= ctHeader.size()) return "";

    if (ctHeader[pos] == '"') {
        // Quoted boundary
        ++pos;
        auto end = ctHeader.find('"', pos);
        if (end == std::string::npos) return ctHeader.substr(pos);
        return ctHeader.substr(pos, end - pos);
    } else {
        // Unquoted boundary — ends at next ; or whitespace or end
        auto end = ctHeader.find_first_of("; \t\r\n", pos);
        if (end == std::string::npos) return ctHeader.substr(pos);
        return ctHeader.substr(pos, end - pos);
    }
}

std::vector<MimePart> EmailMessage::_parseMimeParts(
    const std::multimap<std::string, std::string> &headers,
    const std::string &body)
{
    // Get content-type
    auto ctIt = headers.find("content-type");
    if (ctIt == headers.end()) {
        return {{{"text/plain"}, body, false}};
    }

    std::string ct = _parseContentType(ctIt->second);
    if (ct.empty()) ct = "text/plain";

    // message/rfc822: return only the nested header block
    if (ct == "message/rfc822") {
        std::string nestedHeaders;
        std::istringstream stream(body);
        std::string line;
        while (std::getline(stream, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) break;
            if (!nestedHeaders.empty()) nestedHeaders += "\n";
            nestedHeaders += line;
        }
        return {{ct, nestedHeaders, false}};
    }

    // Non-multipart leaf part
    if (ct.substr(0, 10) != "multipart/") {
        return {{ct, body, false}};
    }

    // multipart/*: split on boundary
    std::string boundary = _parseBoundary(ctIt->second);
    if (boundary.empty()) {
        return {{ct, body, true}};
    }

    std::vector<MimePart> result;
    std::string delimiter = "--" + boundary;
    std::string terminator = "--" + boundary + "--";

    // Split body into segments by delimiter lines
    std::vector<std::string> segments;
    std::istringstream stream(body);
    std::string line;
    std::string currentSegment;
    bool firstDelimiterSeen = false;

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line == terminator) {
            segments.push_back(currentSegment);
            currentSegment.clear();
            // Read remaining as epilogue
            std::string epilogue;
            while (std::getline(stream, line)) {
                if (!line.empty() && line.back() == '\r') line.pop_back();
                if (!epilogue.empty()) epilogue += "\n";
                epilogue += line;
            }
            segments.push_back(epilogue);
            break;
        } else if (line == delimiter) {
            segments.push_back(currentSegment);
            currentSegment.clear();
            firstDelimiterSeen = true;
        } else {
            if (!currentSegment.empty()) currentSegment += "\n";
            currentSegment += line;
        }
    }
    if (!firstDelimiterSeen || (firstDelimiterSeen && segments.size() < 2)) {
        // Malformed: treat whole body as single part
        segments.push_back(currentSegment);
    }

    // segments[0] = prologue, segments[1..n-2] = parts, segments[n-1] = epilogue
    if (!segments.empty()) {
        // Prologue
        result.push_back({ct, segments[0], true});
    }

    // Middle segments are MIME parts
    size_t partEnd = segments.size() > 1 ? segments.size() - 1 : segments.size();
    for (size_t i = 1; i < partEnd; ++i) {
        // Parse sub-headers and sub-body
        std::multimap<std::string, std::string> subHeaders;
        std::string subBody;
        std::istringstream partStream(segments[i]);
        std::string partLine;
        std::string lastSubHeader;
        bool inSubHeaders = true;

        // Skip leading blank lines before sub-headers
        while (inSubHeaders && std::getline(partStream, partLine)) {
            if (!partLine.empty() && partLine.back() == '\r') partLine.pop_back();
            if (partLine.empty()) {
                // End of sub-headers
                inSubHeaders = false;
                break;
            }
            // Continuation
            if ((partLine[0] == ' ' || partLine[0] == '\t') && !lastSubHeader.empty()) {
                auto range = subHeaders.equal_range(lastSubHeader);
                if (range.first != range.second) {
                    auto last = std::prev(range.second);
                    std::string cont = partLine.substr(partLine.find_first_not_of(" \t"));
                    last->second += " " + cont;
                }
                continue;
            }
            auto colonPos = partLine.find(':');
            if (colonPos != std::string::npos) {
                std::string hname = _toLower(partLine.substr(0, colonPos));
                std::string hval = partLine.substr(colonPos + 1);
                auto s = hval.find_first_not_of(" \t");
                if (s != std::string::npos) hval = hval.substr(s);
                auto e = hval.find_last_not_of(" \t\r");
                if (e != std::string::npos) hval = hval.substr(0, e + 1);
                else hval.clear();
                lastSubHeader = hname;
                subHeaders.emplace(hname, hval);
            }
        }

        // Rest is sub-body
        std::string subBodyAccum;
        while (std::getline(partStream, partLine)) {
            if (!partLine.empty() && partLine.back() == '\r') partLine.pop_back();
            if (!subBodyAccum.empty()) subBodyAccum += "\n";
            subBodyAccum += partLine;
        }

        auto subParts = _parseMimeParts(subHeaders, subBodyAccum);
        result.insert(result.end(), subParts.begin(), subParts.end());
    }

    // Epilogue
    if (segments.size() > 1) {
        result.push_back({ct, segments.back(), true});
    }

    return result;
}

} // namespace sieve
