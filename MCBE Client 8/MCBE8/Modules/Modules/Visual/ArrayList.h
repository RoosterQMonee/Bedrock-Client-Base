#pragma once

#include <MCBE8/Modules/Module.h>
#include <MCBE8/Events/Events/RenderEvents.h>
#include <MCBE8/Modules/ModuleManager.h>
#include <Utils/Data/Keys.h>

#include <algorithm>
#include <Utils/Render/Render.h>
#include <Utils/Data/Math.h>

#include <MCBE8/Settings/NumberSetting.h>
#include <MCBE8/Settings/BoolSetting.h>


class ArrayList : public Module {
public:
    static inline float TextSize = 18.0f;
    static inline float Padding = 9.5f;
	static inline float ShadowStrength = 50;

    static inline NumberSetting<float> Scale = NumberSetting<float>("Scale", "Size of the ArrayList", 18.0f, 1.f, 30.f);
    static inline BoolSetting<bool> Shadow = BoolSetting<bool>("Shadow", "Draw a shadow under list?", true);

	ArrayList() : Module(ModuleCategory::Visual, "ArrayList", "Show all active modules") {
		this->SetEnabled(true); // enabled by default (and use events)
	}

	void OnEnabled() override {
	    Dispatcher.listen<ImGuiRenderEvent, &ArrayList::OnImGuiRender>(this);
	}

	void OnDisabled() override {
		Dispatcher.deafen<ImGuiRenderEvent, &ArrayList::OnImGuiRender>(this);
	}

	static void SortModules(std::vector<Module*>& sortedModules) { // Sort the modules by length.
		std::sort(sortedModules.begin(), sortedModules.end(), [](Module* a, Module* b) {
			std::string nameA = a->Name + a->GetModeText(); // First module.
			std::string nameB = b->Name + b->GetModeText(); // Last module.
			float textSizeA = Render::GetTextWidth(&nameA, (TextSize / 10)); // First module's Length.
			float textSizeB = Render::GetTextWidth(&nameB, (TextSize / 10)); // Last module's Lenght.
			return textSizeA > textSizeB; // Sort by the most lenght to lower.
		});
	}

	void OnImGuiRender(ImGuiRenderEvent& event) {
        float sc = Scale.Get().Value;
        if (TextSize != sc) {
            TextSize = sc;
            Padding = sc / 2.315;
        }

		ImDrawList* bg = ImGui::GetBackgroundDrawList();
		if (bg == nullptr) return;

		// Sort module list
		static size_t previousSize;
		static std::vector<Module*> sortedModules;
		static std::vector<Module*> currentModules;

		for (auto const& [cat, mods] : ModuleManager::GetModules())
		{
			for (Module* mod : mods) {
				if (!mod->Enabled && mod->Animation <= 0.01 || mod->ShowInModuleList) continue;
				sortedModules.push_back(mod);
			}
		}

		if (sortedModules.size() != previousSize) {
			SortModules(sortedModules);
			currentModules.clear();
			currentModules = sortedModules;
			previousSize = sortedModules.size();
		}

        // render module list
        const float textHeight = Render::GetTextHeight((TextSize / 10)) * (Padding / 10);
        const float padding = 0.5f * (TextSize / 10);
        float textPosY = -1 + 6.20;
        float easeAnim = 0;

        std::vector<std::tuple<Vector4<float>, std::string, std::string, Vector2<float>, float, float>> shadowDraw;

        for (Module* mod : currentModules) {
            float targetAnim = mod->Enabled ? 1.f : 0.f;
            mod->Animation = Math::animate(targetAnim, mod->Animation, Render::GetDeltaTime() * 15);
            mod->Animation = Math::clamp(mod->Animation, 0.f, 1.f);

            easeAnim = mod->Animation;

            std::string ModuleName = mod->Name;
            std::string ModeName = mod->GetModeText();

            float modWidth = Render::GetTextWidth(&ModuleName, (TextSize / 12));
            float modeWidth = !ModeName.empty() ? Render::GetTextWidth(&ModeName, (TextSize / 12)) : 0.f;
            float modulePosX = (ImGui::GetIO().DisplaySize.x - modWidth - modeWidth - padding) - 10.20;
            
            mod->Position.x = Math::lerp(ImGui::GetIO().DisplaySize.x + 5.f, modulePosX, easeAnim);
            mod->Position.y = textPosY;

            Vector4<float> boxRect(mod->Position.x - padding - 8, textPosY, Render::GetScreenSize().x + 9.5f, textPosY + textHeight);

            ImVec2 min = ImVec2(boxRect.x, boxRect.y);
            ImVec2 max = ImVec2(boxRect.z, boxRect.w);

            if (Shadow.Get().Value)
                ImGui::GetBackgroundDrawList()->AddRectFilled(min, max, ImColor(17, 17, 17, 150));

            shadowDraw.push_back(std::make_tuple(boxRect, ModuleName, ModeName, Vector2<float>(mod->Position.x, mod->Position.y), modWidth, easeAnim));

            textPosY = Math::lerp(textPosY, textPosY + textHeight, easeAnim); // Set a space between arraylist modules.
        }

        int ind = 0;
        for (const auto& pos : shadowDraw) {
            ind++;

            Vector4<float> boxPos = std::get<0>(pos);
            std::string ModuleName = std::get<1>(pos);
            std::string ModeName = std::get<2>(pos);
            Vector2<float> ModulePos = std::get<3>(pos);
            float ModuleWidth = std::get<4>(pos);
            float anim = std::get<5>(pos);

            ImColor col = Render::GetRainbowColor(boxPos.y);

            // Outline {
                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(boxPos.x, boxPos.y), ImVec2(boxPos.x + 2, boxPos.w), col);

                Vector4<float> nboxPos;
                if (ind < shadowDraw.size())
                    nboxPos = std::get<0>(shadowDraw[ind]);
                else
                    nboxPos = Vector4<float>(Render::GetScreenSize().x + 5.5f, boxPos.y, Render::GetScreenSize().x + 5.5f, boxPos.y);

                ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(boxPos.x, boxPos.w), ImVec2(nboxPos.x, boxPos.w + 2), col);
            //}
            // Bar {
            //    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(boxPos.z, boxPos.y), ImVec2(boxPos.z + 4, boxPos.w), col);
            //}


            Render::DrawImText(ModulePos, &ModuleName, ImColor(col.Value.x * 255, col.Value.y * 255, col.Value.z * 255, col.Value.w * 255), (TextSize / 12), anim, true);
            Render::DrawImText(Vector2<float>(ModulePos.x + ModuleWidth, ModulePos.y), &ModeName, ImColor(170, 170, 170), (TextSize / 12), anim, true);
        }

        sortedModules.clear(); // Clear sortedModules
	}
};