#include "pch.h"
#include "stage_factory.hpp"


std::shared_ptr<IGameStage> stage_factory::CreateDungeonLoop(DungeonModel& dungeonModel) {
	auto adventurer = std::make_shared<AdventurerStage>(dungeonModel);
	auto dragon = std::make_shared<DragonStage>(dungeonModel);
	auto roundEnd = std::make_shared<RoundEndStage>(dungeonModel);

	adventurer->SetNextStage(dragon);
	dragon->SetNextStage(roundEnd);
	roundEnd->SetNextStage(adventurer);

	// return first stage
	return adventurer;
}
