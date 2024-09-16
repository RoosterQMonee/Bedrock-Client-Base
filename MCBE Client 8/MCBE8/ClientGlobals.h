#pragma once

#include <Dependencies/kiero/kiero.h>

#include <cstdint>
#include <string>
#include <vector>
#include <map>


struct Version {
	uint8_t Major;
	uint8_t Minor;
};

struct ClientGlobals {
	bool Running{};
	kiero::RenderType::Enum RenderMethod{};
	std::vector<uint64_t> ShutdownKeybind;

	std::map<uint64_t, bool> Keymap;
	std::string CurrentScreen;
};


inline ClientGlobals DEFAULT_CONFIG = ClientGlobals{
	/* Running       */ true,
	/* RenderMethod  */ kiero::RenderType::D3D12,
	/* ShutdownKey   */ {0x11, 'L'},
	/* Keymap        */ std::map<uint64_t, bool>(),
	/* CurrentScreen */ "undefined"
};