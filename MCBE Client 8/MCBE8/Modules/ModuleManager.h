#pragma once

#include <MCBE8/Modules/Module.h>


class ModuleManager {
public:
	static void Init() {}

	static void HandleKeybinds(uint64_t key, bool held);
	static void RegisterModule(Module* mod);
	static std::map<ModuleCategory, std::vector<Module*>> GetModules();
	static std::vector<Module*> GetModulesFromCategory(ModuleCategory category);
	static Module* GetModuleByName(std::string name);

private:
	static inline std::map<ModuleCategory, std::vector<Module*>> Modules = std::map<ModuleCategory, std::vector<Module*>>();
};