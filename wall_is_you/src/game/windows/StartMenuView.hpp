#pragma once
#include "pch.h"
#include "engine/datatypes/IView.hpp"
#include "engine/ServiceLocator.hpp"

class StartMenuView : public IView {
public:
    StartMenuView();
    virtual ~StartMenuView() = default;

    void HandleEvent(const sf::Event& event) override;
    void Update(float dt) override;
    void Render(sf::RenderWindow& window) override;

private:
    sf::Texture m_backgroundTexture;
    std::unique_ptr<sf::Sprite> m_backgroundSprite;

    sf::RectangleShape m_playButton;
    std::unique_ptr<sf::Text> m_buttonText;
    sf::Font m_font;
};