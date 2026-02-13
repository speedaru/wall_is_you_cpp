#include "pch.h"
#include "dungeon_geom.hpp"

#include "game/constants.hpp"


void dungeon_geom::InitTileSize(const sf::Texture& tileTexture) {
	g_tileSize = sf::Vector2f(tileTexture.getSize());
}

sf::Vector2f dungeon_geom::GetScaledTileSize() {
	return g_tileSize * DUNGEON_SCALE_FACTOR;
}

sf::Vector2f dungeon_geom::GetRoomPosScreenCoords(const DungeonLayout& dungeonLayout, const DungeonRoomPos& roomPos, AnchorType anchor) {
	sf::Vector2i layoutStart = GetDungeonLayoutScreenRect(dungeonLayout).position;
	sf::Vector2f scaledTileSize = g_tileSize * DUNGEON_SCALE_FACTOR;

	sf::Vector2f coords(layoutStart.x + roomPos.col * scaledTileSize.x, layoutStart.y + roomPos.row * scaledTileSize.y);
	return AnchorCoords(coords, scaledTileSize, anchor);
}

sf::IntRect dungeon_geom::GetDungeonLayoutScreenRect(const DungeonLayout& dungeonLayout) {
	sf::Vector2i layoutScreenSize = GetDungeonLayoutSize(dungeonLayout, true);
	sf::Vector2i windowSize = sf::Vector2i(WINDOW_SIZE);

	return sf::IntRect(windowSize / 2 - layoutScreenSize / 2, windowSize / 2 + layoutScreenSize / 2);
}

sf::Vector2i dungeon_geom::GetDungeonLayoutSize(const DungeonLayout& dungeonLayout, bool scaledSize) {
	sf::Vector2f tileSize = g_tileSize;
	if (scaledSize) {
		tileSize *= DUNGEON_SCALE_FACTOR;
	}

	return sf::Vector2i(dungeonLayout.width * (int)tileSize.x, dungeonLayout.height * (int)tileSize.y);
}

sf::Vector2f dungeon_geom::AnchorCoords(sf::Vector2f tl, sf::Vector2f size, AnchorType anchor) {
	switch (anchor) {
	default:
	case AnchorType::TopLeft:
		return tl;
	case AnchorType::Center:
		return GetRectCenter(tl, size);
	}
}

sf::Vector2f dungeon_geom::GetRectCenter(sf::Vector2f coords, sf::Vector2f size) {
	return coords + (size / 2.f);
}
