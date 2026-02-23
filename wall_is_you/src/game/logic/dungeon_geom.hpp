#pragma once
#include "pch.h"

#include "engine/ui/geom.hpp"

#include "game/datatypes/DungeonLayout.hpp"


namespace dungeon_geom {
	inline constexpr float DUNGEON_SCALE = 2.f;
	inline sf::Vector2f g_tileSize;

	void InitTileSize(const sf::Texture& tileTexture);

	sf::Vector2f GetScaledTileSize();

	// get screen coords of where roomPos is (top left, or center)
	sf::Vector2f GetScreenFromRoomPos(const DungeonLayout& dungeonLayout, const DungeonRoomPos& roomPos, AnchorType anchor);

	// get room pos from screen coords
	// returns nullptr if out of bounds
	bool GetRoomPosFromScreen(const DungeonLayout& dungeonLayout, sf::Vector2i& screenCoords, DungeonRoomPos* out);

	// calculate rect that dungeon layout should occupy (centered on screen)
	sf::IntRect GetDungeonLayoutScreenRect(const DungeonLayout& dungeonLayout);

	// calculate screen size that dungeon layout will occupy
	sf::Vector2i GetDungeonLayoutSize(const DungeonLayout& dungeonLayout, bool scaledSize);

	// anchor coords based on anchor
	sf::Vector2f AnchorCoords(sf::Vector2f tl, sf::Vector2f size, AnchorType anchor);

	// center coords
	sf::Vector2f GetRectCenter(sf::Vector2f coords, sf::Vector2f size);
}