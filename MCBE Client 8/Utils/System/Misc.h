#pragma once

#include <cstdint>
#include <chrono>

class MiscUtils {
public:
	static uint64_t GetCurrentMs();
	static std::string GetModuleOfAddress(uintptr_t address);
};