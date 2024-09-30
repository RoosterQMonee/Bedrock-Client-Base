#include <MCBE8/MCBE8.h>
#include <Utils/Memory/Detour.h>
#include <Utils/Logger/Logger.h>
#include <Utils/System/Misc.h>


Detour::~Detour() {
    Restore();
}

Detour::Detour(std::string name, uintptr_t addr, void* detour)
{
    this->FuncPtr = reinterpret_cast<void*>(addr);
    this->Name = name;

    const MH_STATUS result = MH_CreateHook(FuncPtr, detour, &Original);

    Logger::Write<LogLevel::INFO>("Detour", ("Created detour for " + name + " at: " + Misc::GetModuleOfAddress(addr) + ", Status: " + MH_StatusToString(result)).c_str());
}

void Detour::Enable() const
{
    if (FuncPtr == nullptr)
    {
        Logger::Write<LogLevel::ERR>("Detour", "Failed to enable detour for " + Name + " because FuncPtr is null");
        return;
    }

    const MH_STATUS result = MH_EnableHook(FuncPtr);
    Logger::Write<LogLevel::DEBUG>("Detour", ("Attempted enable for " + Name + ", Status: " + MH_StatusToString(result)).c_str());
}

void Detour::Restore() const
{
    if (FuncPtr != nullptr)
    {
        MH_STATUS result = MH_DisableHook(FuncPtr);
        Logger::Write<LogLevel::DEBUG>("Detour", ("Disabled detour for " + Name + ", Status: " + MH_StatusToString(result)).c_str());
        result = MH_RemoveHook(FuncPtr);
        Logger::Write<LogLevel::DEBUG>("Detour", ("Restored detour for " + Name + ", Status: " + MH_StatusToString(result)).c_str());
    }
}
