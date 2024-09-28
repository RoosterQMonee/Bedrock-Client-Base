#pragma once

#include <cstdint>
#include <string>
#include <map>


enum SignatureID {
	Keymap = 0,
	Mouse,
	GetFOV
};


class SignatureManager {
public:
	static inline std::map<SignatureID, std::pair<uintptr_t, std::string>> SignatureMap = {};

	static void Init();
	static uintptr_t GetSignatureAddress(SignatureID sig);
};