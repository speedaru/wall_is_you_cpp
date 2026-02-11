#pragma once
#include "pch.h"
#include "engine/datatypes/IGameStage.hpp"


class DungeonModel;

class IDungeonStage : public IGameStage {
public:
	IDungeonStage(DungeonModel& dungeonModel) : m_dungeonModel(dungeonModel) {}

private:
	DungeonModel& m_dungeonModel;
};
