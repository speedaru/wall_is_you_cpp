#include "pch.h"
#include "resource.h"
#include "Game.hpp"

#include "engine/ServiceLocator.hpp"
#include "engine/WindowManager.hpp"
#include "engine/renderer.hpp"

#include "game/datatypes/UICommands.hpp"
#include "game/datatypes/LogicCommands.hpp"
#include "game/datatypes/SharedGameState.hpp"
#include "game/views/StartMenuView.hpp"
#include "game/views/DungeonView.hpp"

#include "utils/logging.hpp"


Game::Game(sf::Vector2u windowSize, const std::string& title)
	: m_sharedGameState(std::make_unique<SharedGameState>()), m_gameSimulation(&m_sharedGameState)
{
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize), title);
	m_window->setFramerateLimit(60);

	// set window icon
	sf::Image icon(std::filesystem::path("assets/game_icon.png"));
	m_window->setIcon(icon);

    // Initial Window is Start Menu
    sp::ServiceLocator::GetWindowManager().Push(std::make_unique<StartMenuView>());

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
	sp::WindowManager& winManager = sp::ServiceLocator::GetWindowManager();
	winManager.ProcessChanges();
}

void Game::RenderFrame() {
	sp::renderer::Render(*m_window, m_clock);
}

void Game::HandleEvent() {
	sp::WindowManager& winManager = sp::ServiceLocator::GetWindowManager();

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
    auto& uiQueue = sp::ServiceLocator::GetUIQueue<UICommand>();
    sp::WindowManager& windowManager = sp::ServiceLocator::GetWindowManager();

	auto pushView = [&](UICommand& cmd) {
		if (!cmd.view.has_value()) {
			throw new std::exception("invalid push view UICommand, view was nullptr. forgot to handle push ?");
		}
		windowManager.Push(std::move(cmd.view.value()));
	};

    UICommand cmd;
    while (uiQueue.TryPop(cmd)) {
        switch (cmd.type) {
		case UICommand::Type::PushDungeonView:
			//m_sharedGameState->ClearGameSnap(); // clear snap shot before creating a new dungeon view
			cmd.view = std::make_unique<DungeonView>(&m_sharedGameState);
			pushView(cmd);
			break;
        case UICommand::Type::PushView:
			pushView(cmd);
            break;
        case UICommand::Type::PopView:
            windowManager.Pop();
            break;
		case UICommand::Type::ExitGame:
			m_gameSimulation.Stop();
			break;
        }
    }
}
