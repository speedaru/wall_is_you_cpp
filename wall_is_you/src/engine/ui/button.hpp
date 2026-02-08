#pragma once
#include "pch.h"
#include "base.hpp"
#include "geom.hpp"

#include "text.hpp" // custom text widget


namespace sp {
	namespace ui {
		class Button : IWidget {
		public:
			void SetBackgroundColor(sf::Color color);
			void SetSize(float w, float h);
			void SetSizeFromText(float innerPadding);
			void SetPos(float x, float y, AnchorType anchorOrigin);
			void SetFont(const std::shared_ptr<sf::Font>& font, uint32_t size, sf::Color color);
			void SetFontColor(sf::Color color);
			void SetText(const std::string& text);

			bool IsClicked(const sf::RenderWindow& window, const sf::Event& event) const;

			virtual void Render(sf::RenderWindow& window) const override;
			virtual sf::FloatRect GetGlobalBounds() const override {
				return m_rect.getGlobalBounds();
			}

		private:
			void CenterText();

		private:
			sf::RectangleShape m_rect;
			Text m_text;
		};
	}
}