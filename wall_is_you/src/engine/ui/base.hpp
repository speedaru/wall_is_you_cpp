#pragma once
#include "pch.h"

namespace sp {
	namespace ui {
		class IWidget {
		public:
			virtual sf::FloatRect GetGlobalBounds() const = 0;

			virtual void Render(sf::RenderWindow& window) const = 0;
		};
	}
}

