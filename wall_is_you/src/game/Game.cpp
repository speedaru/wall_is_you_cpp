#include "pch.h"
#include "Game.hpp"

#include "engine/ServiceLocator.hpp"

#include "game/windows/StartMenuView.hpp"


Game::Game(sf::Vector2u windowSize, const std::string& title) {
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize), title);
    m_window->setFramerateLimit(60);

    // Initial Window is Start Menu
    ServiceLocator::GetWindowManager().Push(std::make_unique<StartMenuView>());
}

void Game::DoFrame() {
	WindowManager& winManager = ServiceLocator::GetWindowManager();
	float dt = m_clock.restart().asSeconds();

	HandleEvents();

	// Process pushes/pops requested by Logic thread or UI
	winManager.ProcessChanges();

	// Update top window
	winManager.Update(dt);

	// Render
	m_window->clear(sf::Color::Black);
	winManager.Render(*m_window);
	m_window->display();
}

void Game::HandleEvents() {
	WindowManager& winManager = ServiceLocator::GetWindowManager();

	std::optional<sf::Event> event = m_window->pollEvent();
	while (event.has_value()) {
		if (event.value().is<sf::Event::Closed>()) {
			m_window->close();
		}
		winManager.HandleEvents(event.value());

		event = m_window->pollEvent();
	}
}
