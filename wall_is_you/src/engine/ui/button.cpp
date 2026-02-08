#include "pch.h"
#include "button.hpp"


void sp::ui::Button::SetSize(float w, float h) {
    m_rect.setSize(sf::Vector2f(w, h));

    CenterText();
}

void sp::ui::Button::SetSizeFromText(float innerPadding) {
    sf::Vector2f textSize = m_text.GetGlobalBounds().size;
    SetSize(textSize.x + innerPadding, textSize.y + innerPadding);
}

void sp::ui::Button::SetPos(float x, float y, AnchorType anchorOrigin) {
    // must set size before pos
    assert(m_rect.getSize() != sf::Vector2f(0.f, 0.f));

    m_rect.setOrigin(geom::GetAnchorPos(m_rect.getLocalBounds().size, anchorOrigin));
    m_rect.setPosition(sf::Vector2f(x, y));

    CenterText();
}

void sp::ui::Button::SetFont(const std::shared_ptr<sf::Font>& font, uint32_t size, sf::Color color) {
    m_text.SetFont(font, size, color);
}

void sp::ui::Button::SetFontColor(sf::Color color) {
    m_text.SetColor(color);
}

void sp::ui::Button::SetText(const std::string& text) {
    assert(m_text.IsInitialized());

    m_text.SetText(text);
}

void sp::ui::Button::SetBackgroundColor(sf::Color color) {
    m_rect.setFillColor(color);
}

bool sp::ui::Button::IsClicked(const sf::RenderWindow& window, const sf::Event& event) const {
    return geom::IsClicked(window, m_rect.getGlobalBounds(), event);
}


void sp::ui::Button::Render(sf::RenderWindow& window) const {
    window.draw(m_rect);
    m_text.Render(window);
}


void sp::ui::Button::CenterText() {
    if (m_text.IsInitialized()) {
        m_text.SetPos(m_rect.getPosition().x, m_rect.getPosition().y, AnchorType::CENTER);
    }
}
