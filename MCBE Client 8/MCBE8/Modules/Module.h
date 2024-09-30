#pragma once

#include <MCBE8/MCBE8.h>
#include <MCBE8/Events/EventHandler.h>
#include <MCBE8/Settings/Setting.h>

#include <cstdint>
#include <vector>
#include <string>


enum ModuleCategory : uint8_t {
	Client = 0,
	Visual
};


class Module {
public:
	Module(std::string& name, std::string& description, ModuleCategory category, uint64_t keybind = 0) : Category{ category }, Name{ name }, Description{ description }, Keybind{ keybind } {
	}

	virtual void OnEnabled() {};
	virtual void OnDisabled() {};

	void SetEnabled(bool enb) {
		this->Enabled = enb;

		if (enb)
			this->OnEnabled();
		else
			this->OnDisabled();
	}

	void Toggle() {
		this->SetEnabled(!this->Enabled);
	}

	void AddSetting(Setting* st) {
		this->Settings.emplace_back(st);
	}

	Setting* GetSettingByName(std::string name) {
		for (Setting* st : this->Settings)
			if (st->Name == name)
				return st;
		return NULL;
	}

	nes::event_dispatcher& Dispatcher = MCBE8::EventDispatcher;

	ModuleCategory Category;
	std::vector<Setting*> Settings;
	std::string Name, Description;
	bool Enabled, ShowInModuleList, HeldKeybind;
	uint64_t Keybind; // Maybe add multi-key keybinds? (CTRL+key ?)
};