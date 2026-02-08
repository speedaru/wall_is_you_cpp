#pragma once
#include "pch.h"

#include "engine/AssetManager.hpp"


enum class AssetId {
	// fonts
	FONT_QUICKSAND,
	FONT_OPEN_SANS,
	FONT_RUBIK,
	FONT_OUTFIT,

	START_BACKGROUND,
};

namespace assets {
	void LoadAssets(AssetManager& assetManager);
}
