#include "pch.h"
#include "assets.hpp"

#include "constants.hpp"


std::unordered_map<AssetId, fs::path> textures = {
	{ AssetId::StartBackground, ASSETS_PATH / "start_background.png" },
	{ AssetId::BlocksGrid, ASSETS_PATH / "blocks_no_bg.png" },
	{ AssetId::BlockBackground, ASSETS_PATH / "block_background.png" },
	{ AssetId::Adventurer, ASSETS_PATH / "adventurer.png" },
	{ AssetId::Dragon, ASSETS_PATH / "dragon.png" },
	{ AssetId::StrongSword, ASSETS_PATH / "strong_sword.png" },
	{ AssetId::ChaosSeal, ASSETS_PATH / "chaos_seal.png" },
	{ AssetId::TreasuresGrid, ASSETS_PATH / "treasures.png" },
};

std::unordered_map<AssetId, fs::path> fonts = {
	{ AssetId::FontQuicksand, FONTS_PATH / "quicksand-SemiBold.ttf" },
	{ AssetId::FontOpenSans, FONTS_PATH / "OpenSans-SemiBold.ttf" },
	{ AssetId::FontRubik, FONTS_PATH / "Rubik-SemiBold.ttf" },
	{ AssetId::FontOutfit, FONTS_PATH / "Outfit-Regular.ttf" },
};


void assets::LoadAssets(sp::AssetManager& assetManager) {
	using Callback_t = void (sp::AssetManager::*)(AssetId, const fs::path&);
	
	auto addAssets = [&](const std::unordered_map<AssetId, fs::path>& assetMap, Callback_t callback) {
		for (const auto& [key, val] : assetMap) {
            LOG_D("loading %s\n", val.string().c_str());
			(assetManager.*callback)(key, val);
		}
	};

	addAssets(textures, &sp::AssetManager::AddTexture);
	addAssets(fonts, &sp::AssetManager::AddFont);
}
