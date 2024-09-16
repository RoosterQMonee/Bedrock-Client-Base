#include <Utils/System/Misc.h>
#include <Utils/Data/String.h>

#include <windows.h>


uint64_t MiscUtils::GetCurrentMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string MiscUtils::GetModuleOfAddress(uintptr_t address) {
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