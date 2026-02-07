#pragma once
#include "pch.h"

#include "engine/datatypes/IView.hpp"

#include "game/views/HudView.hpp"


class DungeonView : public IView {
public:
	DungeonView() : m_hudView(std::make_unique<HudView>()) {}

	virtual bool HandleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
	virtual void Update(float dt) override;
	virtual void Render(sf::RenderWindow& window) override;

    virtual bool IsModal() const override { return true; }

private:
	std::unique_ptr<HudView> m_hudView;
};