#include "pch.h"
#include "TurnController.hpp"

#include "game/datatypes/DungeonModel.hpp"
#include "game/datatypes/IDungeonStage.hpp"


bool TurnController::Update(float dt) {
	bool modified = false;

	// process interrupts first
	if (!m_interrupts.empty()) {
		modified |= m_interrupts.front()->OnUpdate(dt);
		if (m_interrupts.front()->IsFinished()) m_interrupts.pop();
		return modified; // block main turn while interrupts run
	}

	// process current phase
	modified |= m_currentStage->OnUpdate(dt);

	if (m_currentStage->IsFinished()) {
		TransitionToNextPhase();
		modified = true;
	}

	return modified;
}

void TurnController::TransitionToNextPhase() {
}
