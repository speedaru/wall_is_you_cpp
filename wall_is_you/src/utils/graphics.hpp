#include "pch.h"


namespace sp {
	namespace utils {
		namespace graphics {
			// get a specific cell rect from a grid
			sf::IntRect GetTextureRectFromGrid(const sf::Texture& grid, sf::Vector2f cellSize, int textureIdx);

			// set a scale based on a target size, uses sprite local bounds to find size and scale it based on that
			void SetSpriteScale(sf::Sprite& sprite, sf::Vector2f targetSize);
			//void CreateSpriteFromGridCell(const sf::Texture& grid, sf::Vector2u cellSize, int textureIdx, sf::Sprite& outSprite);
		}
	}
}
