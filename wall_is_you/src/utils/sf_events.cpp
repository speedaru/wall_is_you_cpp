#include "pch.h"
#include "sf_events.hpp"

bool sp::utils::IsKeyPressed(const sf::Event& event, sf::Keyboard::Key key) {
	if (auto keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		return keyPressed->code == key;
	}

	return false;
}
