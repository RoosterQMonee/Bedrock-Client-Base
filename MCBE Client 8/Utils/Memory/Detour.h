#pragma once

#include <string>

class Detour {
public:
    std::string Name;

    void* FuncPtr;
    void* Original{};

    ~Detour();
    Detour(std::string name, uintptr_t addr, void* detour);

    void Enable() const;
    void Restore() const;
};