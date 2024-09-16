#pragma once

#include <MCBE8/Hooks/Hook.h>
#include <vector>


class HookManager {
public:
    static std::vector<Hook*> Hooks;

    static void Init();
    static Hook* GetHookByName(std::string name);
    static void Shutdown();
};