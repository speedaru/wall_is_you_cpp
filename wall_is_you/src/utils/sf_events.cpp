#include "pch.h"
#include "sf_events.hpp"

bool sp::utils::ui::IsKeyPressed(const sf::Event& event, sf::Keyboard::Key key) {
	if (auto keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		return keyPressed->code == key;
	}

	return false;
}

bool sp::utils::ui::IsMouseButtonPressed(const sf::Event& event, sf::Mouse::Button button) {
	if (auto mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
		return mousePressed->button == button;
	}

	return false;
}
