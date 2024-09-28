#pragma once

#include <MCBE8/MCBE8.h>
#include <MCBE8/Hooks/Hooks/Game/FOV.h>

#include <SDK/SignatureManager.h>
#include <Utils/Memory/Memory.h>
#include <thread>


std::unique_ptr<Detour> fovCallbackDetour = nullptr;
class LevelRendererPlayer;


void* getFOVDetour(LevelRendererPlayer* self, float a, bool a2) {
    void* original = fovCallbackDetour.get()->Original;
    auto oFunc = Memory::GetFastcall<void*, LevelRendererPlayer*, float, bool>(original);

    return oFunc(self, a, a2);
}


void FOV::Init() {
    uintptr_t keymapFuncAddr = SignatureManager::GetSignatureAddress(SignatureID::GetFOV);
    fovCallbackDetour = std::make_unique<Detour>("KeyMap", keymapFuncAddr, &getFOVDetour);
    fovCallbackDetour->Enable();
    this->Detours.emplace_back(fovCallbackDetour.get());
}


void FOV::UnHook() {
    fovCallbackDetour->Restore();
}