#include "pch.h"
#include "geom.hpp"

sf::Vector2f sp::ui::geom::GetAnchorPos(sf::Vector2f size, AnchorType anchor) {
	switch (anchor) {
	case AnchorType::Center:
		return sf::Vector2f(size.x / 2.f, size.y / 2.f);
	case AnchorType::BottomRight:
		return sf::Vector2f(size.x, size.y);
	case AnchorType::BottomLeft:
		return sf::Vector2f(0.f, size.y);
	case AnchorType::TopRight:
		return sf::Vector2f(size.x, 0.f);
	case AnchorType::TopLeft:
	default:
		return sf::Vector2f(0.f, 0.f);
	}
}

bool sp::ui::geom::IsClicked(const sf::RenderWindow& window, sf::FloatRect rect, const sf::Event& event) {
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);

            // if click occured in button rectangle
            return rect.contains(mousePos);
        }
    }

    return false;
}
