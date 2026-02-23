#include "pch.h"
#include "GameSimulation.hpp"

#include <engine/datatypes/IGameStage.hpp>
#include "engine/ServiceLocator.hpp"
#include "engine/ThreadSafeQueue.hpp"
#include "engine/WindowManager.hpp"

#include "game/datatypes/LogicCommands.hpp"
#include "game/datatypes/SharedGameState.hpp"
#include "stage_factory.hpp"
#include "dungeon_loader.hpp"

#include "utils/logging.hpp"
#include "utils/io.hpp"


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
        bool stateChanged = false;

        // handle commands sent from UI
        //stateChanged |= HandleLogicCommands();
        stateChanged |= HandleLogicCommands();

        // 2. Run the Logic Phase Controller (Adventurer Turn -> Dragon Turn)
        if (!m_isPaused && m_dungeon.IsLoaded()) {
			stateChanged |= UpdateStages(dt);
        }

        // 3. Check for Win/Loss;
        //LOG_D("running game logic\n");

        // finally get game snapshot if state changed
        if (stateChanged) {
			UpdateGameSnapshot();
		}

        // sleep
        auto endTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        if (elapsed < tickRate) {
            std::this_thread::sleep_for(tickRate - elapsed);
        }
    }
}


bool GameSimulation::HandleLogicCommands() {
    bool modified = false;
    auto& logicQueue = sp::ServiceLocator::GetLogicQueue<LogicCommand>();

    LogicCommand cmd;
    while (logicQueue.TryPop(cmd)) {
        switch (cmd.type) {
        case LogicCommand::Type::HandleLoadDungeon:
            HandleLoadDungeon(std::get<LoadDungeonData>(cmd.payload));
            modified = true;
            break;
        case LogicCommand::Type::EntityInteraction:
            break;  
        case LogicCommand::Type::RotateRoom:
            HandleRotateRoom(std::get<RotateRoomData>(cmd.payload));
            modified = true;
            break;
        }
    }

    // not modified state
	return modified;
}

bool GameSimulation::UpdateStages(float dt) {
    bool modified = false;

    // exit interupt stage before current stage
    if (!m_interruptQueue.empty()) {
        auto& interrupt = m_interruptQueue.front();
        modified |= interrupt->OnUpdate(dt);

        if (interrupt->IsFinished()) {
            interrupt->OnExit();
            m_interruptQueue.pop();
        }
    }

    if (m_currentStage) {
        modified |= m_currentStage->OnUpdate(dt);

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

    return modified;
}

void GameSimulation::UpdateGameSnapshot() {
    GameSnapshot gameSnap;

    m_dungeon.GetSnapshot(&gameSnap.dungeonSnap);

    m_sharedGameState->PushGameSnap(std::move(gameSnap));
}


#pragma region logic_commands

void GameSimulation::HandleLoadDungeon(const LoadDungeonData& data) {
	// create stages
	m_currentStage = stage_factory::CreateDungeonLoop(m_dungeon);

    // clear previous dungeon
    m_dungeon.ResetEntitySystem();

    // load dungeon layout
    dungeon_loader::LoadFromFile(data.path, m_dungeon);

    // load entites
    LOG_D("loaded dungeon: %s\n", data.path.string().c_str());
}

void GameSimulation::HandleRotateRoom(const RotateRoomData& data) {
    m_dungeon.RotateRoom(data.roomPos);
}

#pragma endregion
