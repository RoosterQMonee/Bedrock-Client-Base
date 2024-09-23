#pragma once

#include <cstdint>
#include <chrono>


class MiscUtils {
public:
	static uint64_t GetCurrentMs();
	static std::string GetModuleOfAddress(uintptr_t address);
	static std::string GetRoamingPath();

    static std::string RandString(const int len) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i) {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        return tmp_s;
    }
};