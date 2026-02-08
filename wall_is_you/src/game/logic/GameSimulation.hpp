#pragma once
#include "pch.h"

#include "game/datatypes/GameSnapshot.hpp"
#include "game/logic/DungeonModel.hpp"


class IGameStage;
class SharedGameState;
struct LoadDungeonData;

class GameSimulation {
public:
    GameSimulation(std::unique_ptr<SharedGameState>* sharedState) : m_sharedGameState(*sharedState) {
    }

    void Start();
    void Stop();

    bool IsRunning() const {
        return m_isRunning.load();
    }

private:
    void Run(); // The actual thread loop

    void HandleLogicCommands();

    void UpdateStages(float dt);

    void UpdateGameSnapshot();

private: // logic commands
    void LoadDungeon(const LoadDungeonData& data);

private:
    // thread stuff
    std::jthread m_thread;
    std::atomic<bool> m_isRunning{ false };
    std::atomic<bool> m_isPaused{ false };

    // stages
    std::shared_ptr<IGameStage> m_currentStage = nullptr;
    std::queue<std::shared_ptr<IGameStage>> m_interruptQueue;

    // data
    std::unique_ptr<SharedGameState>& m_sharedGameState;
    DungeonModel m_dungeon;
};
