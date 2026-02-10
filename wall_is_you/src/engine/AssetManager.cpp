#include "pch.h"
#include "AssetManager.hpp"


void sp::AssetManager::AddTexture(AssetId id, const fs::path& path) {
	std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
	m_assets[id] = texture;
	bool _ = texture->loadFromFile(path);
}

void sp::AssetManager::AddFont(AssetId id, const fs::path& path) {
	auto font = std::make_shared<sf::Font>();
	if (font->openFromFile(path)) {
		m_assets[id] = font;
	}
}

