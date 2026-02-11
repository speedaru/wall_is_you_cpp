#include "pch.h"
#include "GameSnapshot.hpp"


class SharedGameState {
public:
	void PushGameSnap(GameSnapshot&& snap) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_gameSnap = snap;
		m_isDirt = true;
	}

	// returns true if pulled data
	bool PullGameSnap(GameSnapshot& out) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_isDirt) {
			return false;
		}

		out = std::move(m_gameSnap);
		m_isDirt = false;
		return true;
	}

	// so we can't pull an invalid game snap
	void ClearGameSnap() {
		m_isDirt = false;
	}

private:
	std::mutex m_mutex;

	GameSnapshot m_gameSnap;
	std::atomic<bool> m_isDirt{ false }; // invalid flag so we don't pull same snapshot twice
};