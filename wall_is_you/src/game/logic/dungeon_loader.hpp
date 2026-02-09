#pragma once
#include "pch.h"


class DungeonModel;

namespace dungeon_loader {
	void LoadFromFile(const fs::path& path, DungeonModel& dungeon);
}
