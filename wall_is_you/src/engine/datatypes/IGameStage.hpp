#pragma once
#include "pch.h"


class IGameStage {
public:
	virtual ~IGameStage() = default;

	virtual void OnEnter() = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnExit() = 0;

	// linked list pointer
	void SetNextStage(std::shared_ptr<IGameStage> nextStage) { m_nextStage = nextStage; }
	std::shared_ptr<IGameStage> GetNextStage() const { return m_nextStage; }

	virtual bool IsFinished() const = 0;

private:
	std::shared_ptr<IGameStage> m_nextStage = nullptr;
};