#include <MCBE8/MCBE8.h>

#include <TlHelp32.h>
#include <Psapi.h>
#include <cstdio>

#include <Utils/System/Exceptions.h>
#include <Utils/Logger/Logger.h>
#include <Utils/System/Window.h>
#include <Utils/System/Misc.h>

#include <SDK/SignatureManager.h>
#include <MCBE8/Hooks/HookManager.h>
#include <Utils/Memory/Memory.h>
#include <Utils/System/Files.h>


// forward declaractions
bool InitializeMinHook();
bool InitializeKiero();
void CreateConsole();
void AwaitShutdown();


void MCBE8::Init() {
	MCBE8::ClientVersion = { 1, 2 };
	MCBE8::Globals = DEFAULT_CONFIG;
	MCBE8::Globals.ScreenData = ScreenInfo{ GetSystemMetrics(SM_CYSCREEN), GetSystemMetrics(SM_CXSCREEN) };
	uint64_t StartInjectTime = MiscUtils::GetCurrentMs();

	CreateConsole();
	Exceptions::Init();

	if (!InitializeKiero())
		goto shutdown_client;

	FileUtils::InitFileSystem();
	SignatureManager::Init();
	HookManager::Init();

	Logger::Write<LogLevel::INFO>("Init", "MCBE8 injected in ", MiscUtils::GetCurrentMs() - StartInjectTime, "ms");

	AwaitShutdown();

shutdown_client:
	MCBE8::Shutdown();
}

void MCBE8::Shutdown() {
	Logger::Write<LogLevel::WARN>("Exit", "Shutting down MCBE8");
	
	kiero::unbind(kiero::getRenderType() == kiero::RenderType::D3D12 ? 140 : 8);
	kiero::shutdown();

	Logger::Write<LogLevel::INFO>("Exit", "Disabled Kiero");

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	Logger::Write<LogLevel::INFO>("Exit", "Removed Hooks");

	Sleep(1000); // no crash pls
	FreeLibraryAndExitThread(MCBE8::ModuleHandle, 1);
}


#pragma region nasty init functions

void AwaitShutdown() {
	while (MCBE8::Globals.Running) {
		std::map<uint64_t, bool> keymap = MCBE8::Globals.Keymap;
		std::vector<uint64_t> shutdown_key = MCBE8::Globals.ShutdownKeybind;

		if (std::all_of(
			shutdown_key.begin(), shutdown_key.end(),
			[&keymap](uint64_t key) { return keymap.find(key) != keymap.end();
		})) {
			MCBE8::Globals.Running = false;
			break;
		}

		Sleep(10);
	}

	HookManager::Shutdown();
}


void CreateConsole() {
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

	SetConsoleTitleA(std::format("MCBE8 v{}.{}", MCBE8::ClientVersion.Major, MCBE8::ClientVersion.Minor).c_str());

	MCBE8::Window = FindWindowA(nullptr, "Minecraft");
	if (!MCBE8::Window) MCBE8::Window = FindWindowA(nullptr, "Minecraft: Developer Edition");

	Logger::Write<LogLevel::INFO>("Init", "Allocated Console");
}


bool InitializeMinHook() {
	bool Success = (MH_Initialize() == MH_OK);
	if (Success)
		Logger::Write<LogLevel::INFO>("Init", "Initialized MinHook");
	else
		Logger::Write<LogLevel::ERR>("Init", "Failed to initialize MinHook");
	return Success;
}


bool InitializeKiero() {
	if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success)
		Logger::Write<LogLevel::INFO>("Init", "Loaded Kiero D3D12");
	else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		Logger::Write<LogLevel::INFO>("Init", "Loaded Kiero D3D11");
	else {
		Logger::Write<LogLevel::ERR>("Init", "Could not initialize Kiero (D3D11/D3D12)");
		return false;
	}

	MCBE8::Globals.RenderMethod = kiero::getRenderType();
	return true;
}

#pragma endregion