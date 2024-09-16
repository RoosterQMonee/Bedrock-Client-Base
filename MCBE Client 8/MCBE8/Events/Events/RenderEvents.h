#pragma once

#include <MCBE8/Events/Event.h>


struct ImGuiPreRenderEvent : public Event {};
struct ImGuiPostRenderEvent : public Event {};
struct ImGuiDebugRenderEvent : public Event {};