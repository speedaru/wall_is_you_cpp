#pragma once
#include "pch.h"
namespace fs = std::filesystem;

#include "rendering/SpImage.hpp"


enum class AssetId;

class AssetManager {
public:
	using Asset = std::variant<
		std::shared_ptr<SpImage>,
		std::shared_ptr<sf::Font>
	>;

	AssetManager() = default;

	bool AssetInitialized(AssetId id) const {
		return m_assets.contains(id);
	}

	void AddSprite(AssetId id, const fs::path& path);
	void AddFont(AssetId id, const fs::path& path);

	template <typename T>
	std::shared_ptr<T> GetAsset(AssetId id);

private:
	std::unordered_map<AssetId, Asset> m_assets;
};


template <typename T>
std::shared_ptr<T> AssetManager::GetAsset(AssetId id) {
	if (!AssetInitialized(id)) {
		throw new std::exception("asset id not created");
		return nullptr;
	}

	return std::get<std::shared_ptr<T>>(m_assets[id]);
}
