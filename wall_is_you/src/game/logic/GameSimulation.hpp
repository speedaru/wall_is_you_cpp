#pragma once
#include "pch.h"

class IGameStage;

class GameSimulation {
public:
    void Start();
    void Stop();

    bool IsRunning() const {
        return m_isRunning.load();
    }

private:
    void Run(); // The actual thread loop

    void HandleLogicCommands();

    void UpdateStages(float dt);

private:
    std::jthread m_thread;
    std::atomic<bool> m_isRunning{ false };
    std::atomic<bool> m_isPaused{ false };
    std::shared_ptr<IGameStage> m_currentStage = nullptr;
    std::queue<std::shared_ptr<IGameStage>> m_interruptQueue;
};
