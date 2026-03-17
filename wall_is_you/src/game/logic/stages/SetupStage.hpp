#pragma once
#include "pch.h"

#include "game/datatypes/IDungeonStage.hpp"

class SetupStage : public IDungeonStage {
public:
	using IDungeonStage::IDungeonStage;

	virtual void OnEnter() override;
	virtual bool OnUpdate(float dt) override;
	virtual void OnExit() override;

	virtual bool IsFinished() const;
};