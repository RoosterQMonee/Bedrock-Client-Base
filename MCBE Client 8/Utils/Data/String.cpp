#include <Utils/Data/String.h>
#include <algorithm>
#include <sstream>
#include <regex>
#include <random>


bool String::ContainsIgnoreCase(std::string str, std::string find) {
    std::string mainStrLower = str;
    std::string subStrLower = find;

    std::transform(mainStrLower.begin(), mainStrLower.end(), mainStrLower.begin(), ::tolower);
    std::transform(subStrLower.begin(), subStrLower.end(), subStrLower.begin(), ::tolower);

    return mainStrLower.find(subStrLower) != std::string::npos;
}

std::vector<std::string> String::SplitString(std::string str, std::string delimiter) {
    std::vector<std::string> strings;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        strings.push_back(token);

        str.erase(0, pos + delimiter.length());
    }

    strings.push_back(str);

    return strings;
}

std::vector<std::string> String::SplitString(std::string str, char delimiter) {
    std::vector<std::string> strings;

    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        strings.push_back(token);

        str.erase(0, pos + 1);
    }

    strings.push_back(str);

    return strings;
}

std::string String::ToHex(uintptr_t address) {
    std::stringstream stream;
    stream << std::hex << address;
    std::string result(stream.str());
    return String::ToUpper(result); // copilot sucks and im too lazy to google
}

std::string String::ToLower(const std::string& string)
{
    std::string str = string;
    std::ranges::transform(str, str.begin(), ::tolower);
    return str;
}

std::string String::ToUpper(const std::string& string)
{
    std::string str = string;
    std::ranges::transform(str, str.begin(), ::toupper);
    return str;
}

bool String::IsNumber(std::string str) {
    return std::regex_match(str, std::regex("[-+]?([0-9]*\\.[0-9]+|[0-9]+)"));
}


bool String::InvalidChar(char c) {
    return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

std::string String::SanitizeName(const std::string& name)
{
    std::string out;
    bool wasValid = true;
    for (char c : name) {
        bool isValid = !InvalidChar(c);
        if (wasValid) {
            if (!isValid)
                wasValid = false;

            else
                out += c;
        }
        else
            wasValid = isValid;
    }

    out = RemoveWhitespace(out);
    out = RemoveSymbols(out);

    if (out.find("\n") != std::string::npos)
        out = out.substr(0, out.find("\n"));

    return out;
}

std::string String::RemoveWhitespace(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}

std::string String::RemoveSymbols(std::string str) {
    str.erase(std::remove_if(str.begin(), str.end(), ::ispunct), str.end());
    return str;
}

std::string String::ReplaceAll(std::string str, const char* c1, const char* c2) {
    size_t pos = 0;
    while ((pos = str.find(c1, pos)) != std::string::npos) {
        str.replace(pos, strlen(c1), c2);
        pos += strlen(c2);
    }
    return str;
}

std::string String::ReplaceAll(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t start_pos = 0;
    while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
        result.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return result;
}

std::string String::ReplaceAll(const std::string& str, const std::vector<std::pair<std::string, std::string>>& replacements) {
    std::string result = str;
    for (const auto& [from, to] : replacements)
        result = ReplaceAll(result, from, to);

    return result;
}

std::string String::ReplaceFirst(std::string str, const char* c1, const char* c2)
{
    size_t pos = str.find(c1);
    if (pos != std::string::npos)
        str.replace(pos, strlen(c1), c2);

    return str;
}


std::string String::GenerateUUID() {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);
    std::stringstream ss;
    int i;

    ss << std::hex;
    for (i = 0; i < 8; i++) { ss << dis(gen); }
    ss << "-";
    for (i = 0; i < 4; i++) { ss << dis(gen); }
    ss << "-4";
    for (i = 0; i < 3; i++) { ss << dis(gen); }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) { ss << dis(gen); }
    ss << "-";
    for (i = 0; i < 12; i++) { ss << dis(gen); };

    std::string str = ss.str();
    str.pop_back();
    str.pop_back();
    str += "cc";
    std::string result = String::ToLower(str);
    return result;
}

std::string String::RemoveColorCodes(std::string str) {
    return std::regex_replace(std::regex_replace(str, std::regex("§."), ""), std::regex("&."), "");
}

bool String::StartsWith(std::string str, std::string start) {
    return str.rfind(start, 0) == 0;
}

std::string String::RelativeTime(int64_t ms)
{
    int64_t hours = ms / 3600;
    int64_t minutes = (ms % 3600) / 60;
    int64_t seconds = ms % 60;

    std::stringstream ss;

    if (hours > 0)
        ss << hours << "h ";

    if (minutes > 0 || hours > 0)
        ss << minutes << "m ";

    ss << seconds << "s";

    return ss.str();
}

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

std::string String::Base64Decode(const std::string& input) {
    std::string decoded;
    int in_len = input.length();
    int i = 0;

    while (in_len-- && input[i] != '=') {
        char c1 = base64_chars.find(input[i++]);
        char c2 = base64_chars.find(input[i++]);
        char c3 = base64_chars.find(input[i++]);
        char c4 = base64_chars.find(input[i++]);

        char triple = (c1 << 2) | (c2 >> 4);
        decoded.push_back(triple);

        if (c3 != 64) {
            char sextet = ((c2 & 0x0F) << 4) | (c3 >> 2);
            decoded.push_back(sextet);
        }

        if (c4 != 64) {
            char sextet = ((c3 & 0x03) << 6) | c4;
            decoded.push_back(sextet);
        }
    }

    return decoded;
}

std::vector<uint8_t> String::Base64DecodeBytes(const std::string& input) {
    std::vector<uint8_t> decoded;
    int in_len = input.length();
    int i = 0;

    while (in_len-- && input[i] != '=') {
        char c1 = base64_chars.find(input[i++]);
        char c2 = base64_chars.find(input[i++]);
        char c3 = base64_chars.find(input[i++]);
        char c4 = base64_chars.find(input[i++]);

        char triple = (c1 << 2) | (c2 >> 4);
        decoded.push_back(triple);

        if (c3 != 64) {
            char sextet = ((c2 & 0x0F) << 4) | (c3 >> 2);
            decoded.push_back(sextet);
        }

        if (c4 != 64) {
            char sextet = ((c3 & 0x03) << 6) | c4;
            decoded.push_back(sextet);
        }
    }

    return decoded;
}

std::string String::BytesToHex(const std::vector<uint8_t>& bytes)
{
    std::string hex;
    for (uint8_t byte : bytes)
        hex += String::ToHex(byte) + " ";

    return hex;
}

std::string String::Join(std::vector<std::string> vec, const std::string& string) {
    std::string result;
    for (size_t i = 0; i < vec.size(); i++) {
        result += vec[i];
        if (i != vec.size() - 1)
            result += string;
    }
    return result;
}

std::vector<std::string> String::Split(std::string str, std::string splitter) {
    std::vector<std::string> result;
    size_t pos = 0;

    while ((pos = str.find(splitter)) != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + splitter.length());
    }

    result.push_back(str);
    return result;
}

std::string String::Trim(std::string& str) {
    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);

    return str;
}

std::string String::GetLineContaining(const std::string& string, const char* str)
{
    std::istringstream stream(string);
    std::string line;

    while (std::getline(stream, line))
        if (line.find(str) != std::string::npos)
            return line;

    return "";
}

std::string String::RemoveNonAscii(std::string& str) {
    std::erase_if(str, [](unsigned char c) { return c > 127 && c != '&'; });
    return str;
}
