#pragma once

#include <MCBE8/MCBE8.h>
#include <MCBE8/Hooks/Hooks/Input/Keymap.h>

#include <SDK/SignatureManager.h>
#include <Utils/Memory/Memory.h>
#include <thread>


std::unique_ptr<Detour> keyCallbackDetour = nullptr;


void* keyMapDetour(uint64_t keyId, bool held) {
    void* original = keyCallbackDetour.get()->Original;
    auto oFunc = Memory::GetFastcall<void*, uint64_t, bool>(original);

    MCBE8::Globals.Keymap[keyId] = held;

    return oFunc(keyId, held);
}


void KeyMap::Init() {
    uintptr_t keymapFuncAddr = SignatureManager::GetSignatureAddress(Signatures::Keymap);
    keyCallbackDetour = std::make_unique<Detour>("KeyMap", keymapFuncAddr, &keyMapDetour);
    keyCallbackDetour->Enable();
    this->Detours.emplace_back(keyCallbackDetour.get());
}


void KeyMap::UnHook() {
    keyCallbackDetour->Restore();
}