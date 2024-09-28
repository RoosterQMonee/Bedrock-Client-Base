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
	NumberSetting(std::string name, std::string description, T value, T min, T max) : Name(name), Description(description) {
		assert(constexpr(std::is_arithmetic_v<T>) && "NumberSetting value is not an arithmetic type!");
		this->Config = NumberSettingConfig<T>{ value, min, max };
	}

	NumberSettingConfig<T> Get() {
		return this->Config;
	}

	void Load(NumberSettingConfig<T> conf) {
		this->Config = conf;
	}

	void RenderSettings() override {
		ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - (ImGui::CalcTextSize(this->GetName().c_str()).x * 1.5));
		ImGui::SliderFloat((this->GetName() + "##" + this->differentiator).c_str(), &this->stored, this->minv, this->maxv, this->format);

		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::Text(this->GetDesc().c_str());
			ImGui::EndTooltip();
		}
	}

private:
	NumberSettingConfig<T> Config{};
};