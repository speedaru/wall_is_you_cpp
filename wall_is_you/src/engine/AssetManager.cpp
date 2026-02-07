#include "pch.h"
#include "AssetManager.hpp"


void AssetManager::AddSprite(AssetId id, const fs::path& path) {
	sf::Texture texture;
	if (texture.loadFromFile(path)) {
		m_assets[id] = std::make_shared<SpImage>(texture);
	}
}

void AssetManager::AddFont(AssetId id, const fs::path& path) {
	auto font = std::make_shared<sf::Font>();
	if (font->openFromFile(path)) {
		m_assets[id] = font;
	}
}

