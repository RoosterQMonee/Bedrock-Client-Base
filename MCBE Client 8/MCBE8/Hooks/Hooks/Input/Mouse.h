#pragma once

#include <MCBE8/Hooks/Hook.h>


class Mouse : public Hook {
public:
    Mouse() : Hook("Mouse") {}

    void Init() override;
    void UnHook() override;
};