#include "pch.h"

#include "game/logic/GameSimulation.hpp"

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
	std::unique_ptr<sf::RenderWindow> m_window;
	sf::Clock m_clock;
    GameSimulation m_gameSimulation;
};
