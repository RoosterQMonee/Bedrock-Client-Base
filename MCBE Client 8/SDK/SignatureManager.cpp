#include <SDK/SignatureManager.h>
#include <SDK/Signatures.h>

#include <Utils/Memory/Memory.h>
#include <Utils/Logger/Logger.h>


void SignatureManager::Init() {
	Logger::Write<LogLevel::INFO>("Init", "Scanning signatures");

	SignatureMap[SignatureID::Keymap] = std::make_pair(Memory::ScanSignature(Signatures::Hooks::Keymap).value(), "Keymap");
	SignatureMap[SignatureID::Mouse] = std::make_pair(Memory::ScanSignature(Signatures::Hooks::Mouse).value(), "Mouse");
	SignatureMap[SignatureID::GetFOV] = std::make_pair(Memory::ScanSignature(Signatures::Hooks::FOV).value(), "GetFOV");

	for (std::pair<SignatureID, std::pair<uintptr_t, std::string>> sig : SignatureMap)
		if (sig.second.first == NULL)
			Logger::Write<LogLevel::ERR>("Sigs", "Failed to find: ", sig.second.second);

	Logger::Write<LogLevel::INFO>("Sigs", "Scanned ", SignatureMap.size(), " signatures.");
}


uintptr_t SignatureManager::GetSignatureAddress(SignatureID sig) {
	return SignatureMap[sig].first;
}