#include "pch.h"
#include "resource.h"
#include "Game.hpp"

#include "engine/ServiceLocator.hpp"
#include "engine/WindowManager.hpp"
#include "engine/renderer.hpp"

#include "game/views/StartMenuView.hpp"


Game::Game(sf::Vector2u windowSize, const std::string& title) {
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize), title);
    m_window->setFramerateLimit(60);

	// set window icon
	sf::Image icon(std::filesystem::path("assets/game_icon.png"));
	m_window->setIcon(icon);

    // Initial Window is Start Menu
    ServiceLocator::GetWindowManager().Push(std::make_unique<StartMenuView>());

    m_gameSimulation.Start();
}

Game::~Game() {
    m_gameSimulation.Stop();
}

void Game::DoFrame() {
	HandleEvent();

	// handle ui command queue
	HandleUICommands();

	// Process pushes/pops requested by Logic thread or UI
	WindowManager& winManager = ServiceLocator::GetWindowManager();
	winManager.ProcessChanges();
}

void Game::RenderFrame() {
	sp::engine::renderer::Render(*m_window, m_clock);
}

void Game::HandleEvent() {
	WindowManager& winManager = ServiceLocator::GetWindowManager();

	std::optional<sf::Event> event = m_window->pollEvent();
	while (event.has_value()) {
		if (event.value().is<sf::Event::Closed>()) {
			m_window->close();
		}
		winManager.HandleEvent(*m_window.get(), event.value());

		event = m_window->pollEvent();
	}
}

void Game::HandleUICommands() {
    UICommandQueue& uiQueue = ServiceLocator::GetUIQueue();
    WindowManager& windowManager = ServiceLocator::GetWindowManager();

    UICommand cmd;
    while (uiQueue.TryPop(cmd)) {
        switch (cmd.type) {
        case UICommand::Type::PushView:
            windowManager.Push(std::move(cmd.view));
            break;
        case UICommand::Type::PopView:
            windowManager.Pop();
            break;
        }
    }
}
