#pragma once
#include "pch.h"
#include "engine/datatypes/IGameStage.hpp"

#include "game/datatypes/IDungeonStage.hpp"

#include "utils/logging.hpp"


struct TurnContext {

};

class AdventurerStage : public IDungeonStage {
public:
	using IDungeonStage::IDungeonStage;

	virtual void OnEnter() override {
		LOG_D("entered adventurer stage\n");
	}

	virtual bool OnUpdate(float dt) override {
		return false;
	}

	virtual void OnExit() override {

	}

	virtual bool IsFinished() const {
		return false;
	}
};
