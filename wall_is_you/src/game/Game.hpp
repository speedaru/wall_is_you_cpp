#include "pch.h"


class Game {
public:
	Game(sf::Vector2u windowSize, const std::string& title);

	void DoFrame();

	bool IsRunning() const {
		return m_window->isOpen();
	}

private:
	void HandleEvents();

private:
	std::unique_ptr<sf::RenderWindow> m_window;
	sf::Clock m_clock;
};
