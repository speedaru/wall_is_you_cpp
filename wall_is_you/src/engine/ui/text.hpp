#pragma once
#include "pch.h"
#include "base.hpp"
#include "geom.hpp"

namespace sp {
	namespace ui {
		class Text : IWidget {
		public:
			void SetFont(const std::shared_ptr<sf::Font>& font, uint32_t size, sf::Color color);
			void SetColor(sf::Color color);
			void SetText(const std::string& text);
			void SetPos(float x, float y, AnchorType anchorOrigin);

			bool IsClicked(const sf::RenderWindow& window, const sf::Event& event) const;

			bool IsInitialized() const {
				return m_text != nullptr;
			}

			virtual sf::FloatRect GetGlobalBounds() const override {
				return m_text->getGlobalBounds();
			}
			virtual void Render(sf::RenderWindow& window) const override;

		private:
			std::unique_ptr<sf::Text> m_text;
			std::shared_ptr<sf::Font> m_font;
		};
	}
}
