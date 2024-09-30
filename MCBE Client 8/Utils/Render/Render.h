#pragma once

#pragma warning(disable : 4244) // i dislike this warning.

#include <Dependencies/imgui/imgui.h>
#include <SDK/Math/Vector2.h>
#include <SDK/Math/Vector4.h>

#include <Windows.h>
#include <TlHelp32.h>
#include <chrono>


namespace Render
{
	inline ImColor GetRainbowColor(long off = 0)
	{
		auto now = std::chrono::system_clock::now();
		auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
		auto value = now_ms.time_since_epoch();
		long long duration = value.count();

		float t = ((duration + off) % 6000) / 6000.0f * (5.0f / 6.0f);

		ImVec4 color = ImColor::HSV(t, 0.8f, 0.8f);

		return ImColor(color);
	}

	inline static void DrawImText(Vector2<float> pos, std::string* textStr, const ImColor& color, float textSize, bool shadow = false, int index = 0, ImDrawList* d = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;
		ImFont* font = ImGui::GetFont();

		ImVec2 textPos = ImVec2(pos.x, pos.y);
		constexpr ImVec2 shadowOffset = ImVec2(1.f, 1.f);

		if (shadow)
		{
			d->AddText(font, (textSize * 18), textPos + shadowOffset, ImColor(color.Value.x * 0.03f, color.Value.y * 0.03f, color.Value.z * 0.03f, color.Value.w * 0.9f), textStr->c_str());
		}

		d->AddText(font, (textSize * 18), textPos, color, textStr->c_str());
	};

	template <typename T>
	static void DrawRectangle(Vector4<T> pos, const ImColor& color, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList())
	{
		if (!ImGui::GetCurrentContext())
			return;

		list->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, radius);
	}

	static void DrawRoundRectangle (Vector4<float> pos, const ImDrawFlags& flags, float radius, const ImColor& color, float lineWidth)
	{
		if (!ImGui::GetCurrentContext())
			return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, radius, flags, lineWidth);
	}

	static void DrawGradientOpaqueRectangle(Vector4<float> pos, const ImColor& firstColor, const ImColor& secondColor)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		list->AddRectFilledMultiColor(topLeft, bottomRight,
			firstColor,
			secondColor,
			secondColor,
			firstColor);
	}

	static void DrawGradientRectangle(Vector4<float> pos, const ImColor& firstColor, const ImColor& secondColor)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();

		ImVec2 topLeft = ImVec2(pos.x, pos.y);
		ImVec2 bottomRight = ImVec2(pos.z, pos.w);

		list->AddRectFilledMultiColor(topLeft, bottomRight,
			firstColor,
			secondColor,
			secondColor,
			firstColor);
	}

	static void DrawCircle(Vector2<float> center, float radius, const ImColor& color, int segments)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		list->AddCircleFilled(ImVec2(center.x, center.y), radius, color, segments);
	}

	static inline Vector2<float> GetScreenSize() {
		RECT desktop;
		static HWND window = (HWND)FindWindowA(nullptr, "Minecraft");
		GetWindowRect(window, &desktop);
		int w = desktop.right - desktop.left;
		int h = desktop.bottom - desktop.top;
		// not in fullscreen
		if (desktop.top != 0 || desktop.left != 0) {
			w -= 30;
			h -= 45;
		}
		return Vector2<float>(w, h);
	}


	static inline float GetTextWidth(std::string* textStr, float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, textStr->c_str()).x;
	}

	static inline float GetTextHeight(float textSize)
	{
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, "").y;
	}

	static inline float GetDeltaTime()
	{
		return 0.016f; // update this for however you wanna use dt!!
	}
};