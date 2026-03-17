#pragma once
#include "pch.h"
#include "engine/datatypes/IGameStage.hpp"


class DungeonModel;
class TurnController;

class IDungeonStage : public IGameStage {
public:
	IDungeonStage(TurnController& controller, DungeonModel& dungeonModel)
		: m_controller(controller), m_dungeonModel(dungeonModel) {}

private:
	DungeonModel& m_dungeonModel;
	TurnController& m_controller; // controller that owns this stage
};
