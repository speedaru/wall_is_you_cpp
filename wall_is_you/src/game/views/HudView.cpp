#include "pch.h"
#include "HudView.hpp"

#include "engine/ui/includes.hpp"
#include "engine/ServiceLocator.hpp"
#include "engine/AssetManager.hpp"

#include "game/constants.hpp"
#include "game/assets.hpp"

#include "utils/logging.hpp"


HudView::HudView() {
	CreateWidgets();
}

bool HudView::HandleEvent(const sf::RenderWindow& window, const sf::Event& event) {
	return false;
}

void HudView::Update(float dt) {
}

void HudView::Render(sf::RenderWindow& window) {
	for (const auto& hudElement : m_hudElements) {
		hudElement.text->Render(window);
	}
}


void HudView::CreateWidgets() {
	AssetManager assetManager = ServiceLocator::GetAssetManager();
	auto font = assetManager.GetAsset<sf::Font>(AssetId::FONT_OUTFIT);

	// round indicator
	HudElement roundIndicator;
	roundIndicator.type = HudElement::Type::ROUND_INDICATOR;
	roundIndicator.text = std::make_unique<sp::ui::Text>();
	roundIndicator.text->SetFont(font, 28, sf::Color::White);
	roundIndicator.text->SetText(std::format("round: {}", 1));
	roundIndicator.text->SetPos(12.f, 12.f, AnchorType::TOP_LEFT);
	m_hudElements.push_back(std::move(roundIndicator));
}
