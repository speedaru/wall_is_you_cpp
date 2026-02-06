#pragma once
#include "pch.h"
#include "ThreadSafeQueue.hpp"
#include "WindowManager.hpp"
#include "datatypes/UICommands.hpp"

// Forward declarations of managers we haven't built yet
class AssetManager; 
class Logger;

class ServiceLocator {
public:
    // Static accessors for the global services
    static WindowManager& GetWindowManager() { return *m_windowManager; }
    static ThreadSafeQueue<UICommand>& GetUIQueue() { return *m_uiQueue; }
    
    // Initialization (Called in main.cpp)
    static void Init(WindowManager* wm, ThreadSafeQueue<UICommand>* queue) {
        m_windowManager = wm;
        m_uiQueue = queue;
    }

private:
    static inline WindowManager* m_windowManager = nullptr;
    static inline ThreadSafeQueue<UICommand>* m_uiQueue = nullptr;
};