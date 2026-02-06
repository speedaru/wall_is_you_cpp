#include "pch.h"
#include "StartMenuView.hpp"

StartMenuView::StartMenuView() {
    // Load background
    if (m_backgroundTexture.loadFromFile("assets/start_background.png")) {
        m_backgroundSprite = std::make_unique<sf::Sprite>(m_backgroundTexture);
    }

    // Set up a simple play button (Rect)
    m_playButton.setSize(sf::Vector2f(200.f, 60.f));
    m_playButton.setFillColor(sf::Color::Red);
    m_playButton.setOrigin(sf::Vector2f(100.f, 30.f));
    m_playButton.setPosition(sf::Vector2f(640.f, 400.f)); // Centered on 1280x720

    // Load font and text (ensure you have a font file)
    if (m_font.openFromFile("assets/fonts/quicksand-SemiBold.ttf")) {
        m_buttonText = std::make_unique<sf::Text>(m_font);
        m_buttonText->setString("PLAY");
        m_buttonText->setCharacterSize(30);
        m_buttonText->setFillColor(sf::Color::White);
        // Center text on button
        sf::FloatRect textBounds = m_buttonText->getLocalBounds();
        m_buttonText->setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, 
                                textBounds.position.y + textBounds.size.y / 2.0f));
        m_buttonText->setPosition(m_playButton.getPosition());
    }
}

void StartMenuView::HandleEvent(const sf::Event& event) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        if (event.is<sf::Event::MouseLeft>()) {
            sf::Vector2i mousePosi = sf::Mouse::getPosition();
            sf::Vector2f mousePos(static_cast<float>(mousePosi.x), static_cast<float>(mousePosi.y));
            
            // Check if play button clicked
            if (m_playButton.getGlobalBounds().contains(mousePos)) {
                UICommand cmd;
                cmd.type = UICommand::Type::PushView;
                cmd.viewId = 1; // Assuming 1 is GameView
                
                ServiceLocator::GetUIQueue().Push(cmd);
            }
        }
    }
}

void StartMenuView::Update(float dt) {
    // Optional: add button hover effects here
}

void StartMenuView::Render(sf::RenderWindow& window) {
    window.draw(*m_backgroundSprite);
    window.draw(m_playButton);
    window.draw(*m_buttonText);
}