#pragma once

#include <MCBE8/Hooks/Hook.h>


class KeyMap : public Hook {
public:
    KeyMap() : Hook("KeyMap") {}

    void Init() override;
    void UnHook() override;
};