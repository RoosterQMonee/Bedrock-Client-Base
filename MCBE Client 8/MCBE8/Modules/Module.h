#pragma once

#include <MCBE8/MCBE8.h>
#include <MCBE8/Events/EventHandler.h>

#include <cstdint>
#include <string>


enum ModuleCategory : uint8_t {
	Client = 0,
	Visual
};


class Module {
public:
	Module(std::string& name, std::string& description, ModuleCategory category) : Category{ category }, Name{ name }, Description{ description } {
	}

	nes::event_dispatcher& Dispatcher = MCBE8::EventDispatcher;

	ModuleCategory Category;
	std::string Name, Description;
	bool Enabled, ShowInModuleList;
};