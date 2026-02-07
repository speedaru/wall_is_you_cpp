#pragma once
#include "pch.h"
#include "datatypes/UICommands.hpp"
#include "datatypes/LogicCommands.hpp"

#include "ThreadSafeQueue.hpp"


// Forward declarations of managers we haven't built yet
class WindowManager;
class AssetManager; 

typedef ThreadSafeQueue<UICommand> UICommandQueue;
typedef ThreadSafeQueue<LogicCommand> LogicCommandQueue;

class ServiceLocator {
public:
    // Static accessors for the global services
    static WindowManager& GetWindowManager() { return *m_windowManager; }
    static UICommandQueue& GetUIQueue() { return *m_uiQueue; }
    static LogicCommandQueue& GetLogicQueue() { return *m_logicQueue; }
    static AssetManager& GetAssetManager() { return *m_assetManager; }
    
    // Initialization
    static void Init(WindowManager* wm, UICommandQueue* uiQueue, LogicCommandQueue* logicQueue, AssetManager* assetManager) {
        m_windowManager = wm;
        m_uiQueue = uiQueue;
        m_logicQueue = logicQueue;
        m_assetManager = assetManager;
    }

private:
    static inline WindowManager* m_windowManager = nullptr;
    static inline UICommandQueue* m_uiQueue = nullptr;
    static inline LogicCommandQueue* m_logicQueue = nullptr;
    static inline AssetManager* m_assetManager = nullptr;
};