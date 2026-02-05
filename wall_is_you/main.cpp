#include "pch.h"
#include <utils/logging.hpp>

#include <engine/include/WindowManager.hpp>


int main(int argc, char** argv) {
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280, 720)), "Wall is you");
    WindowManager winManager;

    // Initial Window: The Start Menu
    // winManager.push(std::make_unique<StartMenuView>());

    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        std::optional<sf::Event> event = window.pollEvent();
        while (event.has_value()) {
            if (event.value().is<sf::Event::Closed>()) {
                window.close();
            }
            winManager.HandleEvents(event.value());

			event = window.pollEvent();
        }

        // 1. Process pushes/pops requested by Logic thread or UI
        winManager.ProcessChanges();

        // 2. Update top window
        winManager.Update(dt);

        // 3. Render
        window.clear(sf::Color::Black);
        winManager.Render(window);
        window.display();
    }
}

