#pragma once
#include "pch.h"

#include "game/datatypes/TurnPhase.hpp"
#include "game/datatypes/IDungeonStage.hpp"

class DungeonModel;

class TurnController {
public:
    TurnController(DungeonModel& dungeon);

    // returns true if modified game state
    [[nodiscard]] bool Update(float dt);

    void AddInterrupt(std::unique_ptr<IDungeonStage>& interrupt);

private:
    void TransitionToNextPhase();

    // change to m_currentStage to associated stage with phase
    void SetPhase(TurnPhase phase);

    DungeonModel& m_dungeon;
    uint32_t m_roundCounter{ 0 };
    TurnPhase m_phase;
    std::unique_ptr<IDungeonStage> m_currentStage;
    std::queue<std::unique_ptr<IDungeonStage>> m_interrupts;
};