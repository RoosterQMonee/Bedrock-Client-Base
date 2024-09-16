#pragma once

#include <MCBE8/Hooks/Hook.h>


class FOV : public Hook {
public:
    FOV() : Hook("FOV") {}

    void Init() override;
    void UnHook() override;
};