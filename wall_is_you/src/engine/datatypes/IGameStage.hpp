#pragma once
#include "pch.h"


class IGameStage {
public:
	virtual ~IGameStage() = default;

	virtual void OnEnter() = 0;
	// return true if this update actually changed the game state
	[[nodiscard]] virtual bool OnUpdate(float dt) = 0;
	virtual void OnExit() = 0;

	virtual bool IsFinished() const = 0;
};