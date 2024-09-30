#pragma once

#include <string>

namespace Formatting {
	const char* Symbol = "\xC2\xA7";

	static std::string BASE_CHARACTER = Symbol + std::string("");
	static std::string BLACK = Symbol + std::string("0");
	static std::string DARK_BLUE = Symbol + std::string("1");
	static std::string DARK_GREEN = Symbol + std::string("2");
	static std::string DARK_AQUA = Symbol + std::string("3");
	static std::string DARK_RED = Symbol + std::string("4");
	static std::string DARK_PURPLE = Symbol + std::string("5");
	static std::string GOLD = Symbol + std::string("6");
	static std::string GRAY = Symbol + std::string("7");
	static std::string DARK_GRAY = Symbol + std::string("8");
	static std::string BLUE = Symbol + std::string("9");
	static std::string GREEN = Symbol + std::string("a");
	static std::string AQUA = Symbol + std::string("b");
	static std::string RED = Symbol + std::string("c");
	static std::string LIGHT_PURPLE = Symbol + std::string("d");
	static std::string YELLOW = Symbol + std::string("e");
	static std::string WHITE = Symbol + std::string("f");

	static std::string OBFUSCATED = Symbol + std::string("k");
	static std::string BOLD = Symbol + std::string("l");
	static std::string STRIKETHROUGH = Symbol + std::string("m");
	static std::string UNDERLINE = Symbol + std::string("n");
	static std::string ITALIC = Symbol + std::string("o");
	static std::string RESET = Symbol + std::string("r");
}