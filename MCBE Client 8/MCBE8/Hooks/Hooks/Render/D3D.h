#pragma once

#include <MCBE8/Hooks/Hook.h>


class D3D : public Hook {
public:
    D3D() : Hook("D3D") {}

    void Init() override;
    void UnHook() override;
};