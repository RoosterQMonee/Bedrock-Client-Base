#pragma once

#include <MCBE8/Events/Event.h>
#include <cstdint>

struct ClientKeyEvent : public Event {
	bool Held;
	uint64_t Key;
};

struct ClientMouseEvent : public Event {
	bool IsDown;
	__int8 MouseButton;
	__int16 MouseX;
	__int16 MouseY;
	__int16 RelativeX;
	__int16 RelativeY;
};