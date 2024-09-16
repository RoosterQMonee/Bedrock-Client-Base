#include <MCBE8/Hooks/Hook.h>
#include <Utils/Logger/Logger.h>


Hook::Hook(std::string name, bool initOnConstruct) {
    this->Name = std::move(name);
    InitOnConstruct = initOnConstruct;

    Logger::Write<LogLevel::DEBUG>(this->Name, "Created hook " + this->Name);
}

void Hook::Init() {
    Logger::Write<LogLevel::WARN>(this->Name, "Init was called on " + Name + " but no override was found");
}

void Hook::UnHook() {
    Logger::Write<LogLevel::DEBUG>(this->Name, "UnHook was called on " + Name + " but no override was found");

    for (Detour* detour : Detours) {
        Logger::Write<LogLevel::DEBUG>(this->Name, "Detour: " + detour->Name + " Restored");
        detour->Restore();
    }
}