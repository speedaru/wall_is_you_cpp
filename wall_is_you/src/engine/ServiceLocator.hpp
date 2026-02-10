#pragma once
#include "pch.h"

#include "ThreadSafeQueue.hpp"


namespace sp {
	// Forward declarations of managers we haven't built yet
	class WindowManager;
	class AssetManager; 

	class ServiceLocator {
	public:
		// Static accessors for the global services
		static WindowManager& GetWindowManager() { return *m_windowManager; }
		template <typename T>
		static ThreadSafeQueue<T>& GetUIQueue() { return *(ThreadSafeQueue<T>*)m_uiQueue; }
		template <typename T>
		static ThreadSafeQueue<T>& GetLogicQueue() { return *(ThreadSafeQueue<T>*)m_logicQueue; }
		static AssetManager& GetAssetManager() { return *m_assetManager; }
		
		// Initialization
		template <typename UI_CMD, typename LOGIC_CMD>
		static void Init(WindowManager* wm, ThreadSafeQueue<UI_CMD>* uiQueue, ThreadSafeQueue<LOGIC_CMD>* logicQueue, AssetManager* assetManager) {
			m_windowManager = wm;
			m_uiQueue = uiQueue;
			m_logicQueue = logicQueue;
			m_assetManager = assetManager;
		}

	private:
		static inline WindowManager* m_windowManager = nullptr;
		static inline void* m_uiQueue = nullptr;
		static inline void* m_logicQueue = nullptr;
		static inline AssetManager* m_assetManager = nullptr;
	};
}