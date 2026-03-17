#include "pch.h"
#include "TurnController.hpp"

#include "game/datatypes/DungeonModel.hpp"
#include "game/datatypes/IDungeonStage.hpp"

#include "game/logic/stages/SetupStage.hpp"
#include "game/logic/stages/InputStage.hpp"
#include "game/logic/stages/AdventurerStage.hpp"
#include "game/logic/stages/DragonStage.hpp"
#include "game/logic/stages/RoundEndStage.hpp"

#include "utils/logging.hpp"


TurnController::TurnController(DungeonModel& dungeon)
	: m_dungeon(dungeon)
{
	SetPhase(TurnPhase::Setup);
}

bool TurnController::Update(float dt) {
	if (!m_currentStage) {
		throw std::runtime_error("no stage created");
		return false;
	}

	bool modified = false;

	// process interrupts first
	if (!m_interrupts.empty()) {
		std::unique_ptr<IDungeonStage>& interrupt = m_interrupts.front();
		modified |= interrupt->OnUpdate(dt);
		if (interrupt->IsFinished()) {
			interrupt->OnExit();
			m_interrupts.pop();
		}
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

void TurnController::AddInterrupt(std::unique_ptr<IDungeonStage>& interrupt) {
	interrupt->OnEnter();
	m_interrupts.push(std::move(interrupt));
}


void TurnController::TransitionToNextPhase() {
	m_currentStage->OnEnter();

	// go to next phase and at last phase go back to first one
	switch (m_phase) {
	case TurnPhase::Setup: SetPhase(TurnPhase::Input); break;
	case TurnPhase::Input: SetPhase(TurnPhase::AdventurerPath); break;
	case TurnPhase::AdventurerPath: SetPhase(TurnPhase::DragonMovement); break;
	case TurnPhase::DragonMovement: SetPhase(TurnPhase::Setup); break;
	default:
		throw std::runtime_error("unknown phase");
	}

	if (m_phase == TurnPhase::Setup) {
		m_roundCounter++;
		LOG_D("starting new round: %u\n", m_roundCounter);
	}
}

void TurnController::SetPhase(TurnPhase phase) {
	m_phase = phase;

	switch (m_phase) {
	case TurnPhase::Setup:
		m_currentStage = std::make_unique<SetupStage>(*this, m_dungeon);
		break;
	case TurnPhase::Input:
		m_currentStage = std::make_unique<InputStage>(*this, m_dungeon);
		break;
	case TurnPhase::AdventurerPath:
		m_currentStage = std::make_unique<AdventurerStage>(*this, m_dungeon);
		break;
	case TurnPhase::DragonMovement:
		m_currentStage = std::make_unique<DragonStage>(*this, m_dungeon);
		break;
	default:
		throw std::runtime_error("unknown phase\n");
		return;
	}

	m_currentStage->OnEnter();
}
