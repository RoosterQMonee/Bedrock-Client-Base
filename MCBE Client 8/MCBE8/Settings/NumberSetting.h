#pragma once

#include <Dependencies/imgui/imgui.h>
#include <MCBE8/Settings/Setting.h>

#include <string>
#include <cstdint>



template <typename T>
struct NumberSettingConfig {
	T Value;
	T Min;
	T Max;
};


template <typename T>
class NumberSetting : public Setting {
public:
	NumberSetting(std::string name, std::string description, T value, T min, T max) {
		this->Name = name;
		this->Description = description;
		this->Config = NumberSettingConfig<T>{ value, min, max };
	}

	NumberSettingConfig<T> Get() {
		return this->Config;
	}

	void Load(NumberSettingConfig<T> conf) {
		this->Config = conf;
	}

	void RenderSettings() override {
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (ImGui::CalcTextSize(this->Name.c_str())).x * 1.5);
		ImGui::SliderFloat(std::string(this->Name + "##" + this->differentiator).c_str(), &this->Config.Value, this->Config.Min, this->Config.Min, "%.2f");

		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text(this->Description.c_str());
			ImGui::EndTooltip();
		}
	}

private:
	NumberSettingConfig<T> Config{};
};