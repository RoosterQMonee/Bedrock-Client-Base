#include <Utils/System/Misc.h>
#include <Utils/Data/String.h>

#include <windows.h>


std::string Misc::GetRoamingPath()
{
    char* path = nullptr;
    size_t length;

    _dupenv_s(&path, &length, "appdata");

    if (path == nullptr)
        return "";

    return std::string(path) + "\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState";
};

uint64_t Misc::GetCurrentMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string Misc::GetModuleOfAddress(uintptr_t address) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((void*)address, &mbi, sizeof(mbi))) {
        char moduleName[MAX_PATH];
        if (GetModuleFileNameA((HMODULE)mbi.AllocationBase, moduleName, sizeof(moduleName))) {
            uintptr_t moduleBase = (uintptr_t)mbi.AllocationBase;

            std::string modName = moduleName;
            std::string result = modName + "+" + String::ToHex((address - moduleBase));

            return result.substr(result.find_last_of("\\") + 1);
        }
    }
    return std::string(String::ToHex(address));
}

std::string Misc::RandString(const int len) {
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