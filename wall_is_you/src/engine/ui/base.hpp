#pragma once
#include "pch.h"

namespace sp {
	namespace ui {
		class IWidget {
		public:
			virtual sf::Vector2f GetSize() const = 0;
			virtual sf::Vector2f GetPos() const = 0;

			virtual void Render(sf::RenderWindow& window) const = 0;
		};
	}
}

