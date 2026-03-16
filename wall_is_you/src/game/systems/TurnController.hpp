#include "pch.h"

class IDungeonStage;

class TurnController {
public:
    // returns true if modified game state
    bool Update(float dt);

private:
    void TransitionToNextPhase();

    std::unique_ptr<IDungeonStage> m_currentStage;
    std::queue<std::unique_ptr<IDungeonStage>> m_interrupts;
};