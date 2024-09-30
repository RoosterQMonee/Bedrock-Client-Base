#include <MCBE8/Modules/ModuleManager.h>
#include <Utils/Logger/Logger.h>

#include <MCBE8/Modules/Modules/Client/ClickGUI.h>
#include <MCBE8/Modules/Modules/Visual/ArrayList.h>


#define ADD_MODULE(mod) ModuleManager::RegisterModule(new mod())


void ModuleManager::RegisterModule(Module* mod) {
	std::vector<Module*> &list = Modules.at(mod->Category);
	list.emplace_back(mod);

	Logger::WriteCR<LogLevel::DEBUG>("Mods", "Loading module: ", mod->Name, "                        ");
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
	Modules.insert({ ModuleCategory::Client, std::vector<Module*>() });
	Modules.insert({ ModuleCategory::Visual, std::vector<Module*>() });

	ADD_MODULE(ClickGUI);
	ADD_MODULE(ArrayList);

	Logger::Write<LogLevel::DEBUG>("Modules", "Loaded modules                        "); // erase previous lines
}