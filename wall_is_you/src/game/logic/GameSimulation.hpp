#pragma once
#include "pch.h"

#include "game/datatypes/GameSnapshot.hpp"
#include "game/datatypes/DungeonModel.hpp"

#include "game/systems/TurnController.hpp"


class IGameStage;
class SharedGameState;
struct LoadDungeonData;
struct RotateRoomData;

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

    // returns true if state changed
    [[nodiscard]] bool HandleLogicCommands();
    // returns true if state changed
    [[nodiscard]] bool UpdateStages(float dt);

    void UpdateGameSnapshot();

    // logic commands
    void HandleLoadDungeon(const LoadDungeonData& data);
    void HandleRotateRoom(const RotateRoomData& data);

private:
    // thread and state stuff
    std::jthread m_thread;
    std::atomic<bool> m_isRunning{ false };
    bool m_isPaused{ false };

    // data
    std::unique_ptr<SharedGameState>& m_sharedGameState;
    DungeonModel m_dungeon;

    //// stages
    //std::shared_ptr<IGameStage> m_currentStage = nullptr;
    //std::queue<std::shared_ptr<IGameStage>> m_interruptQueue;
    std::unique_ptr<TurnController> m_turnController;
};
