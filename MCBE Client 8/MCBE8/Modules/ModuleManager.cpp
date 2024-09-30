#include <MCBE8/Modules/ModuleManager.h>


#define ADD_MODULE(mod) ModuleManager::RegisterModule(new mod())


void ModuleManager::RegisterModule(Module* mod) {
	Modules.at(mod->Category).emplace_back(mod);
}

std::map<ModuleCategory, std::vector<Module*>> ModuleManager::GetModules() {
	return Modules;
}

std::vector<Module*> ModuleManager::GetModulesFromCategory(ModuleCategory category) {
	return Modules.at(category);
}

Module* ModuleManager::GetModuleByName(std::string name) {
	for (auto const& [cat, mods] : Modules)
	{
		for (Module* mod : mods)
			if (mod->Name == name)
				return mod;
	}
	return NULL;
}

void ModuleManager::HandleKeybinds(uint64_t k, bool h) {
	for (auto const& [cat, mods] : Modules)
	{
		for (Module* mod : mods)
			if ((mod->Keybind == k))
				if (mod->HeldKeybind)
					mod->SetEnabled(h);
				else if (h)
					mod->Toggle();
	}
}

void ModuleManager::Init() {

}