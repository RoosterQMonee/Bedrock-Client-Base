#pragma once

#pragma comment(lib, "D:/Global/libMinHook-x64-v141-md.lib") // you'll probably need to change this
#include <Dependencies/minhook/Minhook.h>
#include <Dependencies/kiero/kiero.h>
#include <MCBE8/Events/EventHandler.h>
#include <MCBE8/ClientGlobals.h>

#include <Windows.h>
#include <cstdint>


class MCBE8 {
public:
    static inline bool Initialized;
    static inline Version ClientVersion;

    static inline HWND Window;
    static inline HMODULE ModuleHandle;
    static inline ClientGlobals Globals;

    static inline nes::event_dispatcher EventDispatcher;

    static void Init();
    static void Shutdown();
};

