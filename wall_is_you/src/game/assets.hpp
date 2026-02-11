#pragma once
#include "pch.h"

#include "engine/AssetManager.hpp"


enum class AssetId {
	// fonts
	FontQuicksand,
	FontOpenSans,
	FontRubik,
	FontOutfit,

	// blocks
	StartBackground,
	BlockTileset,
	BlockBackground,

	// entities
	Adventurer,
	Dragon,
	Treasure,
	StrongSword,
	ChaosSeal,
};

namespace assets {
	void LoadAssets(sp::AssetManager& assetManager);
}
