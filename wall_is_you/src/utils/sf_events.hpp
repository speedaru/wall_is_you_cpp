#pragma once
#include "pch.h"


namespace sp {
	namespace utils {
		namespace ui {
			bool IsKeyPressed(const sf::Event& event, sf::Keyboard::Key key);
			bool IsMouseButtonPressed(const sf::Event& event, sf::Mouse::Button button);
		}
	}
}
