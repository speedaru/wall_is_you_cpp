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
	BlocksGrid,
	BlockBackground,

	// entities
	Adventurer,
	Dragon,
	TreasuresGrid,
	StrongSword,
	ChaosSeal,
};

namespace assets {
	void LoadAssets(sp::AssetManager& assetManager);
}
