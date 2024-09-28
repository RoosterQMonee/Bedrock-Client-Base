#pragma once

#include <MCBE8/MCBE8.h>
#include <MCBE8/Hooks/Hooks/Input/Mouse.h>
#include <MCBE8/Events/Events/InputEvents.h>

#include <SDK/SignatureManager.h>
#include <Utils/Memory/Memory.h>
#include <thread>


std::unique_ptr<Detour> mouseCallbackDetour = nullptr;


void mouseDetour(void* _this, __int8 mouseButton, bool isDown, __int16 mouseX, __int16 mouseY, __int16 relX, __int16 relY, int a8)
{
    if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();
        MCBE8::Globals.MousePosition = ImVec2(mouseX, mouseY);

        switch (mouseButton)
        {
        case 1:
            io.MouseDown[0] = isDown;
            break;

        case 2:
            io.MouseDown[1] = isDown;
            break;

        case 3:
            io.MouseDown[2] = isDown;
            break;

        case 4:
            io.MouseWheel = isDown > 136 ? -0.5f : 0.5f;

            break;
        }

        if (io.WantCaptureMouse)
            return;
    }

    auto event = nes::make_holder<ClientMouseEvent>(false, isDown, mouseButton, mouseX, mouseY, relX, relY);
    MCBE8::EventDispatcher.trigger(event);

    void* original = mouseCallbackDetour.get()->Original;
    auto oFunc = Memory::GetFastcall<void, void*, __int8, bool, __int16, __int16, __int16, __int16, int>(original);

    oFunc(_this, mouseButton, isDown, mouseX, mouseY, relX, relY, a8);
}


void Mouse::Init() {
    uintptr_t mouseFuncAddr = SignatureManager::GetSignatureAddress(SignatureID::Mouse);
    mouseCallbackDetour = std::make_unique<Detour>("Mouse", mouseFuncAddr, &mouseDetour);
    mouseCallbackDetour->Enable();
    this->Detours.emplace_back(mouseCallbackDetour.get());
}


void Mouse::UnHook() {
    mouseCallbackDetour->Restore();
}