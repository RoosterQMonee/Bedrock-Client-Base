#include <SDK/SignatureManager.h>

#include <Utils/Memory/Memory.h>
#include <Utils/Logger/Logger.h>


void SignatureManager::Init() {
	Logger::Write<LogLevel::INFO>("Init", "Scanning signatures");

	SignatureMap[Signatures::Keymap] = std::make_pair(Memory::ScanSignature("48 83 EC ? ? ? C1 4C 8D 05").value(), "Keymap");
	SignatureMap[Signatures::Mouse] = std::make_pair(Memory::ScanSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24").value(), "Mouse");
	SignatureMap[Signatures::GetFOV] = std::make_pair(Memory::ScanSignature("48 8B C4 48 89 58 ? 48 89 70 ? 57 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 48 8B 05").value(), "GetFOV");

	for (std::pair<Signatures, std::pair<uintptr_t, std::string>> sig : SignatureMap)
		if (sig.second.first == NULL)
			Logger::Write<LogLevel::ERR>("Sigs", "Failed to find: ", sig.second.second);

	Logger::Write<LogLevel::INFO>("Sigs", "Scanned ", SignatureMap.size(), " signatures.");
}


uintptr_t SignatureManager::GetSignatureAddress(Signatures sig) {
	return SignatureMap[sig].first;
}