#include "pch.h"
#include "graphics.hpp"

#include "utils/logging.hpp"


sf::IntRect sp::utils::graphics::GetTextureRectFromGrid(const sf::Texture& grid, sf::Vector2f cellSize, int textureIdx) {
    int columns = grid.getSize().x / (int)cellSize.x;

    int x = (textureIdx % columns) * (int)cellSize.x;
	int y = (textureIdx / columns) * (int)cellSize.y;

    return sf::IntRect({x, y}, sf::Vector2i(cellSize));
}

void sp::utils::graphics::SetSpriteScale(sf::Sprite& sprite, sf::Vector2f targetSize) {
	sf::Vector2f textureSize = sprite.getLocalBounds().size;
	sprite.setScale(sf::Vector2f(targetSize.x / textureSize.x, targetSize.y / textureSize.y));
}

//void sp::utils::graphics::CreateSpriteFromGridCell(const sf::Texture& grid, sf::Vector2u cellSize, int textureIdx, sf::Sprite& outSprite) {
//    outSprite = sf::Sprite(grid);
//    outSprite.setTextureRect(GetTextureRectFromGrid(grid, cellSize, textureIdx));
//}
