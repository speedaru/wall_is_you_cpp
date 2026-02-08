#pragma once
#include "pch.h"


namespace keybinds {
	using namespace sf::Keyboard;
	#define CREATE_BIND(name, val) inline const Key name = val

	CREATE_BIND(EXIT_KEY, Key::Escape);
}
