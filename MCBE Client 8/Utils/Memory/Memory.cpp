#include "Memory.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <future>
#include <execution>



void Memory::PatchBytes(void* dst, void* src, unsigned int size) {
    DWORD oldprotect;         // RWX perms
    VirtualProtect(dst, size, 0x40, &oldprotect);
    memcpy(dst, src, size);
    VirtualProtect(dst, size, oldprotect, &oldprotect);
}


inline uintptr_t Memory::SignatureOffset(uintptr_t sig, int offset) {
    return sig + offset + 4 + *reinterpret_cast<int*>(sig + offset);
}


std::vector<std::optional<uint8_t>> Memory::PatternToBytes(std::string pattern) {
    const char* cpattern = pattern.c_str(); // yuh.

    auto bytes = std::vector<std::optional<uint8_t>>{};
    auto start = const_cast<char*>(cpattern);
    auto end = const_cast<char*>(cpattern) + strlen(cpattern);
    bytes.reserve(strlen(cpattern) / 2);

    for (auto current = start; current < end; ++current) {
        if (*current == '?') {
            ++current;
            if (*current == '?')
                ++current;

            bytes.push_back(std::nullopt);
        }
        else bytes.push_back((uint8_t)strtoul(current, &current, 16));
    }

    return bytes;
}


std::optional<std::uintptr_t> Memory::ScanSignature(std::string signature) {
    static auto gameModule = GetModuleHandleA("Minecraft.Windows.exe");
    auto* const scanBytes = reinterpret_cast<uint8_t*>(gameModule);
    auto* const dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(gameModule);
    auto* const ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(scanBytes + dosHeader->e_lfanew);
    const auto sizeOfCode = ntHeaders->OptionalHeader.SizeOfImage;

    const auto pattern = PatternToBytes(signature);
    const auto end = scanBytes + sizeOfCode;

    auto it = std::search(std::execution::par, scanBytes, end, pattern.cbegin(), pattern.cend(),
    [](auto byte, auto opt) {
        return !opt.has_value() || *opt == byte;
    });

    auto ret = it != end ? (uintptr_t)it : 0u;
    return ret;
}