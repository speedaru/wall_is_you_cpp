#include "pch.h"
#include "DungeonView.hpp"

#include "engine/ServiceLocator.hpp"
#include "engine/WindowManager.hpp"

#include "game/constants.hpp"

#include "utils/logging.hpp"
#include "utils/sf_events.hpp"


bool DungeonView::HandleEvent(const sf::RenderWindow& window, const sf::Event& event) {
	m_hudView->HandleEvent(window, event);

	if (sp::utils::IsKeyPressed(event, keybinds::EXIT_KEY)) {
		UICommand cmd;
		cmd.type = UICommand::Type::PopView;
		ServiceLocator::GetUIQueue().Push(std::move(cmd));
	}

	return false;
}

void DungeonView::Update(float dt) {
	m_hudView->Update(dt);
}

void DungeonView::Render(sf::RenderWindow& window) {
	m_hudView->Render(window);

	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Green);
	rect.setSize(sf::Vector2f(200.f, 200.f));
	rect.setOrigin(sf::Vector2f(rect.getSize().x / 2.f, rect.getSize().y));
	rect.setPosition(sf::Vector2f(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y / 2.f));

	window.draw(rect);
}
