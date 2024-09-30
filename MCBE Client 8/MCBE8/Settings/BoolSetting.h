#pragma once

#include <Dependencies/imgui/imgui.h>
#include <MCBE8/Settings/Setting.h>

#include <string>
#include <cstdint>



template <typename T>
struct BoolSettingConfig {
	T Value;
};


template <typename T>
class BoolSetting : public Setting {
public:
	BoolSetting(std::string name, std::string description, T value) {
		this->Name = name;
		this->Description = description;
		this->Config = BoolSettingConfig<T>{ value };
	}

	BoolSettingConfig<T> Get() {
		return this->Config;
	}

	void Load(BoolSettingConfig<T> conf) {
		this->Config = conf;
	}

	void RenderSettings() override {
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (ImGui::CalcTextSize(this->Name.c_str()).x * 1.5));
		ImGui::Checkbox(this->Name.c_str(), &this->Config.Value);

		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text(this->Description.c_str());
			ImGui::EndTooltip();
		}
	}

private:
	BoolSettingConfig<T> Config{};
};