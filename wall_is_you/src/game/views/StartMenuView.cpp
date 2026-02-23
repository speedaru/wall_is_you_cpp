#include "pch.h"
#include "StartMenuView.hpp"

#include "engine/ServiceLocator.hpp"

#include "game/datatypes/UICommands.hpp"
#include "game/datatypes/LogicCommands.hpp"
#include "game/assets.hpp"
#include "game/constants.hpp"

#include "utils/logging.hpp"
#include "utils/sf_events.hpp"


StartMenuView::StartMenuView() {
    CreateAssets();
    CreateWidgets();
}

bool StartMenuView::HandleEvent(const sf::RenderWindow& window, const sf::Event& event) {
    if (sp::utils::ui::IsKeyPressed(event, keybinds::EXIT_KEY)) {
		UICommand uiCmd;
		uiCmd.type = UICommand::Type::ExitGame;
		sp::ServiceLocator::GetUIQueue<UICommand>().Push(std::move(uiCmd));
        return true;
    }

    // check dungeon file buttons clicked
    for (const auto& [dungeonFile, button] : m_dungeonButtons) {
        if (button.IsClicked(window, event)) {
            UICommand uiCmd;
            uiCmd.type = UICommand::Type::PushDungeonView;
            sp::ServiceLocator::GetUIQueue<UICommand>().Push(std::move(uiCmd));

            LogicCommand logicCmd;
            logicCmd.type = LogicCommand::Type::HandleLoadDungeon;
            logicCmd.payload = LoadDungeonData(dungeonFile);
            sp::ServiceLocator::GetLogicQueue<LogicCommand>().Push(std::move(logicCmd));

			return true;
        }
    }

    return false;
}

void StartMenuView::Update(float dt) {
    // Optional: add button hover effects here
}

void StartMenuView::Render(sf::RenderWindow& window) {
    // background
    assert(m_background != nullptr);
	window.draw(*m_background);

    // dungeon file names
    for (const auto& [dungeonFile, button] : m_dungeonButtons) {
        button.Render(window);
    }

    //// play button
    //m_playButton.Render(window);
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


void StartMenuView::CreateAssets() {
    sp::AssetManager& assetManger = sp::ServiceLocator::GetAssetManager();

    auto backgroundTexture = assetManger.GetAsset<sf::Texture>(AssetId::StartBackground);
    m_background = std::make_unique<sf::Sprite>(*backgroundTexture);
}

void StartMenuView::CreateWidgets() {
    sp::AssetManager& assetManger = sp::ServiceLocator::GetAssetManager();

    auto quicksand = assetManger.GetAsset<sf::Font>(AssetId::FontQuicksand);
    auto outfit = assetManger.GetAsset<sf::Font>(AssetId::FontOutfit);

    // play button
    m_playButton.SetBackgroundColor(sf::Color::Red);
    m_playButton.SetSize(200.f, 60.f);
    m_playButton.SetFont(quicksand, 32, sf::Color::White);
    m_playButton.SetText("PLAY");
    m_playButton.SetPos(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y / 2.f, AnchorType::Center);

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
        button.SetPos(WINDOW_SIZE.x / 2.f, y, AnchorType::Center);
        button.SetBackgroundColor(sf::Color(255, 255, 255, 65));
        
        y += button.GetGlobalBounds().size.y + 20.f;
    }
}
