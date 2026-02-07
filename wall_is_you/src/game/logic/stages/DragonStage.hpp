#pragma once
#include "pch.h"
#include "engine/datatypes/IGameStage.hpp"

class DragonStage : public IGameStage {
public:
	virtual void OnEnter() override {
		
	}

	virtual void OnUpdate(float dt) override {

	}

	virtual void OnExit() override {

	}

	virtual bool IsFinished() const {
		return false;
	}
};
