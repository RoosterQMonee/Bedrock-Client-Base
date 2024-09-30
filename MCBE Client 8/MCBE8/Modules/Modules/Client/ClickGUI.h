#pragma once

#include <MCBE8/Modules/Module.h>
#include <MCBE8/Events/Events/RenderEvents.h>
#include <MCBE8/Modules/ModuleManager.h>
#include <Utils/Data/Keys.h>


class ClickGUI : public Module {
public:
	ClickGUI() : Module(ModuleCategory::Client, "ClickGUI", "MCBE8's ClickGUI", Keys::INSERT) {
		this->SetEnabled(true); // enabled by default
	}

	void OnEnabled() override {
		MCBE8::Globals.RenderUI = true;
		this->Dispatcher.listen<ImGuiRenderEvent, &ClickGUI::OnImGuiRender>(this);
	}

	void OnDisabled() override {
		MCBE8::Globals.RenderUI = false;
		this->Dispatcher.deafen<ImGuiRenderEvent, &ClickGUI::OnImGuiRender>(this);
	}

	void OnImGuiRender(ImGuiRenderEvent& event) {
		for (auto const& [cat, mods] : ModuleManager::GetModules())
		{

		}
	}
};