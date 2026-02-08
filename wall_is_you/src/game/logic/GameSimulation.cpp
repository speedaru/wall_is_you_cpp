#include "pch.h"
#include "GameSimulation.hpp"

#include <engine/datatypes/IGameStage.hpp>
#include "engine/ServiceLocator.hpp"
#include "engine/ThreadSafeQueue.hpp"
#include "engine/WindowManager.hpp"

#include "StageFactory.hpp"

#include "utils/logging.hpp"


void GameSimulation::Start() {
	m_isRunning = true;
	m_thread = std::jthread(&GameSimulation::Run, this);
}

void GameSimulation::Stop() {
    m_isRunning = false;
}

void GameSimulation::Run() {
	const std::chrono::milliseconds tickRate(16); // ~60 Update ticks per second
    const float dt = 1.f / 60.f;
    
    while (m_isRunning) {
        auto startTime = std::chrono::steady_clock::now();

        // handle commands sent from UI
        HandleLogicCommands();

        // 2. Run the Logic Phase Controller (Adventurer Turn -> Dragon Turn)
        if (!m_isPaused) {
			UpdateStages(dt);
        }

        // 3. Check for Win/Loss;
        //LOG_D("running game logic\n");

        // sleep
        auto endTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        if (elapsed < tickRate) {
            std::this_thread::sleep_for(tickRate - elapsed);
        }
    }
}


void GameSimulation::LoadDungeon(const LoadDungeonData& data) {
	// create stages
	m_currentStage = StageFactory::CreateDungeonLoop();

    // load dungeon layout

    // load entites
}

void GameSimulation::HandleLogicCommands() {
    LogicCommandQueue& logicQueue = ServiceLocator::GetLogicQueue();

    LogicCommand cmd;
    while (logicQueue.TryPop(cmd)) {
        switch (cmd.type) {
        case LogicCommand::Type::LoadDungeon:
            LoadDungeon(std::get<LoadDungeonData>(cmd.payload));
            break;
        case LogicCommand::Type::EntityInteraction:
            break;
        }
    }
}

void GameSimulation::UpdateStages(float dt) {
    // exit interupt stage before current stage
    if (!m_interruptQueue.empty()) {
        auto& interrupt = m_interruptQueue.front();
        interrupt->OnUpdate(dt);

        if (interrupt->IsFinished()) {
            interrupt->OnExit();
            m_interruptQueue.pop();
        }
        return;
    }

    if (m_currentStage) {
        m_currentStage->OnUpdate(dt);

        if (m_currentStage->IsFinished()) {
            m_currentStage->OnExit();

            // move to next stage in link
            m_currentStage = m_currentStage->GetNextStage();

            // enter next stage
            if (m_currentStage) {
                m_currentStage->OnEnter();
            }
        }
    }
}
