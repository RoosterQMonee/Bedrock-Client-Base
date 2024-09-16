#pragma once

#include <Dependencies/kiero/kiero.h>
#include <Dependencies/imgui/imgui.h>

#include <cstdint>
#include <string>
#include <vector>
#include <map>


struct Version {
	uint8_t Major;
	uint8_t Minor;
};


struct ScreenInfo {
	int32_t ScreenHeight;
	int32_t ScreenWidth;
};


struct ClientGlobals {
	bool Running{};
	bool RenderUI{};
	ScreenInfo ScreenData{};
	kiero::RenderType::Enum RenderMethod{};
	std::vector<uint64_t> ShutdownKeybind;

	ImVec2 MousePosition{};
	std::map<uint64_t, bool> Keymap;
	std::string CurrentScreen;
};


inline ClientGlobals DEFAULT_CONFIG = ClientGlobals{
	/* Running       */ true,
	/* RenderUI      */ true,
	/* ScreenInfo    */ {0,0},
	/* RenderMethod  */ kiero::RenderType::D3D12,
	/* ShutdownKey   */ {0x11, 'L'}, // VK_CTRL and 76
	/* MousePosition */ {0,0},
	/* Keymap        */ std::map<uint64_t, bool>(),
	/* CurrentScreen */ "undefined"
};