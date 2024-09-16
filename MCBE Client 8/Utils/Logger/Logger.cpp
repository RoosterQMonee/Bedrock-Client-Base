#include <Utils/Logger/Logger.h>


std::unordered_map<LogLevel, std::string> Logger::levelPrefixes = {
    {LogLevel::DEBUG, "[DEBUG]"},
    {LogLevel::INFO, "[INFO]"},
    {LogLevel::WARN, "[WARN]"},
    {LogLevel::ERR, "[ERROR]"}
};
