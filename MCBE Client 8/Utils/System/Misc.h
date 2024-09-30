#pragma once

#include <cstdint>
#include <chrono>


class Misc {
public:
	static uint64_t GetCurrentMs();
	static std::string GetModuleOfAddress(uintptr_t address);
	static std::string GetRoamingPath();
	static std::string RandString(const int len);
};