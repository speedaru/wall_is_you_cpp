#pragma once
#include "pch.h"
#include "renderer.hpp"

#include "ServiceLocator.hpp"
#include "WindowManager.hpp"


void sp::engine::renderer::Render(sf::RenderWindow& window, sf::Clock& clock) {
	WindowManager& winManager = ServiceLocator::GetWindowManager();
	float dt = clock.restart().asSeconds();

	// Update top window
	winManager.Update(dt);

	// Render
	window.clear(sf::Color::Black);
	winManager.Render(window);
	window.display();
}
