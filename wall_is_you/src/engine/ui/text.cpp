#include "pch.h"
#include "text.hpp"


void sp::ui::Text::SetFont(const std::shared_ptr<sf::Font>& font, uint32_t size, sf::Color color) {
    m_font = font;
    m_text = std::make_unique<sf::Text>(*m_font);

    m_text->setCharacterSize(size);
    m_text->setFillColor(color);
}

void sp::ui::Text::SetColor(sf::Color color) {
    m_text->setFillColor(color);
}

void sp::ui::Text::SetText(const std::string& text) {
    assert(m_text != nullptr); // SetText was called before SetFont
    m_text->setString(text);
}

void sp::ui::Text::SetPos(float x, float y, AnchorType anchorOrigin) {
    sf::FloatRect bounds = m_text->getLocalBounds();
	m_text->setOrigin(bounds.position + geom::GetAnchorPos(bounds.size, anchorOrigin));
	m_text->setPosition(sf::Vector2f(x, y));
}

bool sp::ui::Text::IsClicked(const sf::RenderWindow& window, const sf::Event& event) const {
    return geom::IsClicked(window, m_text->getLocalBounds(), event);
}


void sp::ui::Text::Render(sf::RenderWindow& window) const {
    window.draw(*m_text);
}

