#include "pch.h"
#include "DungeonView.hpp"

#include "engine/ServiceLocator.hpp"
#include "engine/WindowManager.hpp"
#include "engine/AssetManager.hpp"

#include "game/datatypes/UICommands.hpp"
#include "game/datatypes/SharedGameState.hpp"
//#include "game/datatypes/DungeonSnapshot.hpp"
#include "game/constants.hpp"
#include "game/assets.hpp"

#include "utils/logging.hpp"
#include "utils/sf_events.hpp"
#include "utils/macros.hpp"


bool DungeonView::HandleEvent(const sf::RenderWindow& window, const sf::Event& event) {
	m_hudView->HandleEvent(window, event);

	if (sp::utils::IsKeyPressed(event, keybinds::EXIT_KEY)) {
		UICommand cmd;
		cmd.type = UICommand::Type::PopView;
		ServiceLocator::GetUIQueue<UICommand>().Push(std::move(cmd));
	}

	return false;
}

void DungeonView::Update(float dt) {
	m_hudView->Update(dt);
}

void DungeonView::Render(sf::RenderWindow& window) {
	// get new game snapshot to render
    m_sharedGameState->PullGameSnap(m_gameSnap);
    DungeonSnapshot& dungeonSnap = m_gameSnap.dungeonSnap;
    DungeonLayout& layout = dungeonSnap.layout;

	// render dungeon layout
    for (DungeonLayoutIterator it = layout; !it.Finished(); it++) {
		DungeonRoom* tileData = *it;
		DungeonRoomPos roomPos = it.GetRoomPos();

		// crop tilset to current tile
		m_tileSet->setTextureRect(GetTextureRect(tileData->type));

		// rotate tile
		m_tileSet->setRotation(sf::degrees(tileData->rotations * 90.f));

		// render background + tile
		RenderSpriteInRoom(window, m_tileBg, TILE_SCALED_SIZE, roomPos);
		RenderSpriteInRoom(window, m_tileSet, TILE_SCALED_SIZE, roomPos);
    }

	// render hud on top
	m_hudView->Render(window);
}


void DungeonView::RenderSpriteInRoom(sf::RenderWindow& window, std::unique_ptr<sf::Sprite>& sprite, sf::Vector2f spriteSize, DungeonRoomPos roomPos) {
	sprite->setScale(sf::Vector2f(SCALE_FACTOR, SCALE_FACTOR));

	float posX = (roomPos.col * spriteSize.x) + (spriteSize.x / 2.f);
	float posY = (roomPos.row * spriteSize.y) + (spriteSize.y / 2.f);

	sprite->setPosition({ posX, posY });
	window.draw(*sprite);
}

void DungeonView::InitSprites() {
    AssetManager& assetManager = ServiceLocator::GetAssetManager();

	auto loadSprite = [&](AssetId id, std::unique_ptr<sf::Sprite>& sprite) {
		if (!sprite) {
			auto texture = assetManager.GetAsset<sf::Texture>(id);
			assert(texture != nullptr);

			sprite = std::make_unique<sf::Sprite>(*texture);
			assert(sprite != nullptr);
		}
	};

	loadSprite(AssetId::BlockBackground, m_tileBg);
	m_tileBg->setOrigin(TILE_CENTER);

	loadSprite(AssetId::BlockTileset, m_tileSet);
	m_tileSet->setOrigin(TILE_CENTER);
}

sf::IntRect DungeonView::GetTextureRect(DungeonTileType type) {
    // We cast the enum to an int to get the index (0 through 5)
    int index = static_cast<int>(type);
    
    // Calculate row and column
    int columns = 3; // Your tileset is 192px wide (192 / 64 = 3)
    int x = (index % columns) * TILE_SIZE;
	int y = (index / columns) * TILE_SIZE;

    return sf::IntRect({x, y}, {TILE_SIZE, TILE_SIZE});
}
