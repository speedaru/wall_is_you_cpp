#pragma once
#include "pch.h"
#include "engine/datatypes/IGameStage.hpp"

#include "game/datatypes/IDungeonStage.hpp"


class AdventurerStage : public IDungeonStage {
public:
	using IDungeonStage::IDungeonStage;

	virtual void OnEnter() override {
		
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
