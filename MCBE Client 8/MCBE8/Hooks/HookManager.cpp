#pragma once

#include <MCBE8/Hooks/HookManager.h>
#include <Utils/Logger/Logger.h>


#include "Hooks/Input/Keymap.h"
#include "Hooks/Game/FOV.h"
#include "Hooks/Render/D3D.h"


std::vector<Hook*> HookManager::Hooks = std::vector<Hook*>();


void HookManager::Init() {
    Logger::Write<LogLevel::INFO>("Init", "Initializing hooks");

    // TODO: wait for MC ClientInstance?
    Hooks.emplace_back(new D3D());
    Hooks.emplace_back(new KeyMap());
    Hooks.emplace_back(new FOV());

    for (Hook* hook : Hooks)
        if (hook->InitOnConstruct && hook->Name != "UIRenderCtx")
        {
            Logger::Write<LogLevel::DEBUG>("Hooks", "Initializing: " + hook->Name);
            hook->Init();
        }

    Logger::Write<LogLevel::INFO>("Hooks", "Initialized " + std::to_string(Hooks.size()) + " hooks");
}

Hook* HookManager::GetHookByName(std::string name) {
    for (Hook* hook : Hooks)
        if (hook->Name == name)
            return hook;

    return nullptr;
}

void HookManager::Shutdown() {
    Logger::Write<LogLevel::WARN>("Hooks", "Unhooking all hooks...");

    for (Hook* hook : Hooks) {
        Logger::Write<LogLevel::DEBUG>("Hooks", "Unhooked, " + hook->Name);
        hook->UnHook();
    }
}