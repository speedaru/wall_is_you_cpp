#pragma once
#include "pch.h"

#include "engine/ui/geom.hpp"

#include "game/datatypes/DungeonLayout.hpp"


namespace dungeon_geom {
	inline constexpr float DUNGEON_SCALE_FACTOR = 2.f;
	inline sf::Vector2f g_tileSize;

	void InitTileSize(const sf::Texture& tileTexture);

	sf::Vector2f GetScaledTileSize();

	// get screen coords of where roomPos is (top left, or center)
	sf::Vector2f GetRoomPosScreenCoords(const DungeonLayout& dungeonLayout, const DungeonRoomPos& roomPos, AnchorType anchor);

	// calculate rect that dungeon layout should occupy (centered on screen)
	sf::IntRect GetDungeonLayoutScreenRect(const DungeonLayout& dungeonLayout);

	// calculate screen size that dungeon layout will occupy
	sf::Vector2i GetDungeonLayoutSize(const DungeonLayout& dungeonLayout, bool scaledSize);

	// anchor coords based on anchor
	sf::Vector2f AnchorCoords(sf::Vector2f tl, sf::Vector2f size, AnchorType anchor);

	// center coords
	sf::Vector2f GetRectCenter(sf::Vector2f coords, sf::Vector2f size);
}