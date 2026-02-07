#pragma once
#include "pch.h"

#include "stages/AdventurerStage.hpp"
#include "stages/DragonStage.hpp"
#include "stages/RoundEndStage.hpp"


class StageFactory {
public:
	static std::shared_ptr<IGameStage> CreateDungeonLoop() {
		auto adventurer = std::make_shared<AdventurerStage>();
		auto dragon = std::make_shared<DragonStage>();
		auto roundEnd = std::make_shared<RoundEndStage>();

		adventurer->SetNextStage(dragon);
		dragon->SetNextStage(roundEnd);
		roundEnd->SetNextStage(adventurer);

		// return first stage
		return adventurer;
	}
};