#include "pch.h"
#include "assets.hpp"

#include "constants.hpp"


std::unordered_map<AssetId, fs::path> sprites = {
	{ AssetId::START_BACKGROUND, ASSETS_PATH / "start_background.png" },
};

std::unordered_map<AssetId, fs::path> fonts = {
	{ AssetId::FONT_QUICKSAND, FONTS_PATH / "quicksand-SemiBold.ttf" },
	{ AssetId::FONT_OPEN_SANS, FONTS_PATH / "OpenSans-SemiBold.ttf" },
	{ AssetId::FONT_RUBIK, FONTS_PATH / "Rubik-SemiBold.ttf" },
	{ AssetId::FONT_OUTFIT, FONTS_PATH / "Outfit-Regular.ttf" },
};


void assets::LoadAssets(AssetManager& assetManager) {
	using Callback_t = void (AssetManager::*)(AssetId, const fs::path&);
	
	auto addAssets = [&](const std::unordered_map<AssetId, fs::path>& assetMap, Callback_t callback) {
		for (const auto& [key, val] : assetMap) {
			(assetManager.*callback)(key, val);
		}
	};

	addAssets(sprites, &AssetManager::AddTexture);
	addAssets(fonts, &AssetManager::AddFont);
}
