#pragma once
#include "pch.h"


enum class AnchorType {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTER
};

namespace sp {
	namespace ui{
		namespace geom {
			sf::Vector2f GetAnchorPos(sf::Vector2f size, AnchorType anchor);
			inline sf::Vector2f GetAnchorPos(float w, float h, AnchorType anchor) {
				return GetAnchorPos(sf::Vector2f(w, h), anchor);
			}

			bool IsClicked(const sf::RenderWindow& window, sf::FloatRect rect, const sf::Event& event);
		}
	}
}
