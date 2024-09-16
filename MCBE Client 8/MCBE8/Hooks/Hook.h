#pragma once

#include <Utils/Memory/Detour.h>
#include <vector>
#include <utility>


class Hook {
public:
    std::vector<Detour*> Detours;
    std::string Name = "Hook";
    bool InitOnConstruct = true;

    Hook(std::string name = "Hook", bool initOnConstruct = true);

    virtual void Init();
    virtual void UnHook();
};
