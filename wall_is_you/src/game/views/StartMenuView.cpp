#include "pch.h"
#include "StartMenuView.hpp"

#include "engine/ServiceLocator.hpp"

#include "game/assets.hpp"
#include "game/constants.hpp"
#include "game/views/DungeonView.hpp"

#include "utils/logging.hpp"


StartMenuView::StartMenuView() {
    CreateWidgets();
}

bool StartMenuView::HandleEvent(const sf::RenderWindow& window, const sf::Event& event) {
    if (m_playButton.IsClicked(window, event)) {
        UICommandQueue& uiQueue = ServiceLocator::GetUIQueue();

		UICommand loadDungeonView;
		loadDungeonView.type = UICommand::Type::PushView;
		loadDungeonView.view = std::make_unique<DungeonView>();

		uiQueue.Push(std::move(loadDungeonView));
        LOG_D("pushed dungeon view\n");

        return true;
    }

    // check dungeon file buttons clicked
    for (const auto& [dungeonFile, button] : m_dungeonButtons) {
        if (button.IsClicked(window, event)) {
            UICommand cmd;
            cmd.type = UICommand::Type::PushView;
            cmd.view = nullptr; // dungeon

            ServiceLocator::GetUIQueue().Push(std::move(cmd));

            return true;
        }
    }

    return false;
}

void StartMenuView::Update(float dt) {
    // Optional: add button hover effects here
}

void StartMenuView::Render(sf::RenderWindow& window) {
    AssetManager& assetManger = ServiceLocator::GetAssetManager();
    const auto& background = assetManger.GetAsset<SpImage>(AssetId::START_BACKGROUND);

    // background
    if (background)
		window.draw(background->sprite);

    // dungeon file names
    for (const auto& [dungeonFile, button] : m_dungeonButtons) {
        button.Render(window);
    }

    // play button
    m_playButton.Render(window);
}


static void GetDungeons(std::vector<fs::path>& outDungeonFiles) {
    fs::directory_iterator it(DUNGEONS_PATH);
    for (const auto& entry : it) {
        if (!entry.is_regular_file()) {
            continue;
        }

        outDungeonFiles.push_back(entry.path());
    }
}

void StartMenuView::CreateWidgets() {
    AssetManager& assetManger = ServiceLocator::GetAssetManager();

    auto quicksand = assetManger.GetAsset<sf::Font>(AssetId::FONT_QUICKSAND);
    auto outfit = assetManger.GetAsset<sf::Font>(AssetId::FONT_OUTFIT);

    // play button
    m_playButton.SetBackgroundColor(sf::Color::Red);
    m_playButton.SetSize(200.f, 60.f);
    m_playButton.SetFont(quicksand, 32, sf::Color::White);
    m_playButton.SetText("PLAY");
    m_playButton.SetPos(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y / 2.f, AnchorType::CENTER);

    // dungeons
    std::vector<fs::path> dungeonFiles;
    GetDungeons(dungeonFiles);
    float y = WINDOW_SIZE.y / 6.f;
    for (const auto& dungeonFile : dungeonFiles) {
        m_dungeonButtons.push_back(DungeonFileButton(dungeonFile, sp::ui::Button()));
        sp::ui::Button& button = m_dungeonButtons.back().button;

        button.SetFont(outfit, 36, sf::Color::White);
        button.SetText(dungeonFile.filename().string());
        button.SetSizeFromText(8.f);
        button.SetPos(WINDOW_SIZE.x / 2.f, y, AnchorType::CENTER);
        button.SetBackgroundColor(sf::Color(255, 255, 255, 65));
        
        y += button.GetGlobalBounds().size.y + 20.f;
    }
}
