#pragma once

#include <cstdint>
#include <string>
#include <map>


enum Signatures {
	Keymap = 0,
	Mouse,
	GetFOV
};


class SignatureManager {
public:
	static inline std::map<Signatures, std::pair<uintptr_t, std::string>> SignatureMap = {};

	static void Init();
	static uintptr_t GetSignatureAddress(Signatures sig);
};