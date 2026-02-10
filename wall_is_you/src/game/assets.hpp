#pragma once
#include "pch.h"

#include "engine/AssetManager.hpp"


enum class AssetId {
	// fonts
	FontQuicksand,
	FontOpenSans,
	FontRubik,
	FontOutfit,

	StartBackground,
	BlockTileset,
	BlockBackground,
	//BlockSolid,
	//BlockSingle,
	//BlockDoubleAdj,
	//BlockDoubleOpp,
	//BlockTriple,
	//BlockQuand
};

namespace assets {
	void LoadAssets(sp::AssetManager& assetManager);
}
