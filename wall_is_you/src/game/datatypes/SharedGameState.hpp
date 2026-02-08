#include "pch.h"
#include "GameSnapshot.hpp"


class SharedGameState {
public:
	void PushGameSnap(GameSnapshot&& snap) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_gameSnap = snap;
		m_hasGameSnap = true;
	}

	// returns true if pulled data
	bool PullGameSnap(GameSnapshot& out) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (!m_hasGameSnap) {
			return false;
		}

		out = std::move(m_gameSnap);
		m_hasGameSnap = false;
		return true;
	}

	// so we can't pull an invalid game snap
	void ClearGameSnap() {
		m_hasGameSnap = false;
	}

private:
	std::mutex m_mutex;

	GameSnapshot m_gameSnap;
	std::atomic<bool> m_hasGameSnap{ false };
};