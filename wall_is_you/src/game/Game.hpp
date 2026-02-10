#include "pch.h"

#include "engine/Renderer.hpp"

#include "game/logic/GameSimulation.hpp"


class SharedGameState;

class Game {
public:
	Game(sf::Vector2u windowSize, const std::string& title);

	~Game();

	void DoFrame();
	void RenderFrame();

	bool IsRunning() const {
		return m_window->isOpen() && m_gameSimulation.IsRunning();
	}

private:
	void HandleEvent();
	void HandleUICommands();

private:
	// window stuff
	std::unique_ptr<sf::RenderWindow> m_window;
	sf::Clock m_clock;

	// data
	std::unique_ptr<SharedGameState> m_sharedGameState;

	// owned classes
    GameSimulation m_gameSimulation;
};
