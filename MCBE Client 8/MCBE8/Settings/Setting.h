#pragma once

#include <string>
#include <Utils/System/Misc.h>


class Setting {
public:
	std::string Name;
	std::string Description;

	std::string differentiator = Misc::RandString(24);

	virtual void RenderSettings() {};
};