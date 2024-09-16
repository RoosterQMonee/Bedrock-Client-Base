#pragma once

#include <iostream>
#include <string_view>
#include <format>
#include <unordered_map>
#include <cstdarg>


// i spent literal hours on some shitty logger class just for it to be std::cout.

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERR
};


class Logger {
private:
    static std::unordered_map<LogLevel, std::string> levelPrefixes;

public:
    template<LogLevel ll, class T, class ... U >
    static void Write(std::string sender, const T& first, const U&... rest)
    {
        std::cout << levelPrefixes[ll] << "[" << sender << "] " << first;
        ((std::cout << rest), ...);
        std::cout << std::endl;
    }
};
