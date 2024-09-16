#pragma once

#pragma comment(lib, "D:/Global/libMinHook-x64-v141-md.lib")
#include <Dependencies/minhook/Minhook.h>
#include <Dependencies/kiero/kiero.h>
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

    static void Init();
    static void Shutdown();
};

