#pragma once
#include "pch.h"

#include "stages/AdventurerStage.hpp"
#include "stages/DragonStage.hpp"
#include "stages/RoundEndStage.hpp"


class DungeonModel;

namespace stage_factory {
	std::shared_ptr<IGameStage> CreateDungeonLoop(DungeonModel& dungeonModel);
}
