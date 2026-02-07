#pragma once
#include "pch.h"
#include "engine/datatypes/IView.hpp"
#include "engine/ui/includes.hpp"


class StartMenuView : public IView {
    struct DungeonFileButton {
        std::filesystem::path file;
        sp::ui::Button button;
    };

public:
    StartMenuView();
    virtual ~StartMenuView() = default;

    bool HandleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
    void Update(float dt) override;
    void Render(sf::RenderWindow& window) override;

    virtual bool IsModal() const override { return true; }

private:
    void CreateWidgets();

// widgets
private:
    sp::ui::Button m_playButton;
    std::vector<DungeonFileButton> m_dungeonButtons;
};