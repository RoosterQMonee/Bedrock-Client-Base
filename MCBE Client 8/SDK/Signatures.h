#pragma once

#include <string>


namespace Signatures {
	namespace Hooks {
		inline std::string Keymap = "48 83 EC ? ? ? C1 4C 8D 05";
		inline std::string Mouse = "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24";
		inline std::string FOV = "48 8B C4 48 89 58 ? 48 89 70 ? 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05";
	}

	namespace SDK {
		inline std::string Packet_CreatePacket = "40 53 48 83 EC 30 45 33 C0 48 8B D9 81 FA ? ? ? ?";
	}
}