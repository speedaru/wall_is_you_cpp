#pragma once
#include "pch.h"
#include "engine/datatypes/IView.hpp"

#include "engine/ui/includes.hpp"


class HudView : public IView {
	struct HudElement {
        enum class Type {
            ROUND_INDICATOR,
        } type;
        std::unique_ptr<sp::ui::Text> text;
	};

    struct HudData {
        int round{};
    };

public:
	HudView();

    virtual bool HandleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
    virtual void Update(float dt) override;
    virtual void Render(sf::RenderWindow& window) override;

    virtual bool IsTransparent() const { return true; }

private:
    void CreateWidgets();

// widgets
private:
    std::vector<HudElement> m_hudElements;
    HudData m_data;
};

