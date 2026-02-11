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
		sp::ServiceLocator::GetUIQueue<UICommand>().Push(std::move(cmd));
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

	// render dungeon layout
    DungeonLayout& layout = dungeonSnap.layout;
    for (DungeonLayoutIterator it = layout; !it.Finished(); it++) {
		RenderRoom(window, *it, it.GetRoomPos());
    }

	// render entities
	EntitySystemSnapshot& entitiesSnap = dungeonSnap.entitySystem;
	for (const auto& entitySnap : entitiesSnap) {
		RenderEntity(window, entitySnap);
	}

	// render hud on top
	m_hudView->Render(window);
}


void DungeonView::RenderSpriteInRoom(sf::RenderWindow& window, std::unique_ptr<sf::Sprite>& sprite, sf::Vector2f targetSpriteSize, DungeonRoomPos roomPos) {
	sf::Vector2i textureSize = sprite->getTextureRect().size;
	sprite->setScale(sf::Vector2f(targetSpriteSize.x / textureSize.x, targetSpriteSize.y / textureSize.y));

	float posX = (roomPos.col * targetSpriteSize.x) + (targetSpriteSize.x / 2.f);
	float posY = (roomPos.row * targetSpriteSize.y) + (targetSpriteSize.y / 2.f);

	sprite->setPosition({ posX, posY });
	window.draw(*sprite);
}

void DungeonView::RenderRoom(sf::RenderWindow& window, DungeonRoom* tileData, DungeonRoomPos roomPos) {
	// crop tilset to current tile
	m_tileSet->setTextureRect(GetTileTextureRect(tileData->type));

	// rotate tile
	m_tileSet->setRotation(sf::degrees(tileData->rotations * 90.f));

	// render background + tile
	sf::Vector2f tileScaledSize = s_tileSize * DUNGEON_SCALE_FACTOR;
	RenderSpriteInRoom(window, m_tileBg, tileScaledSize, roomPos);
	RenderSpriteInRoom(window, m_tileSet, tileScaledSize, roomPos);
}

void DungeonView::RenderEntity(sf::RenderWindow& window, const EntitySnapshot& entitySnap) {
	std::shared_ptr<sf::Texture> texture;
	sf::Vector2f targetSize;
	GetEntityTexture(entitySnap.type, texture, targetSize);

	auto entitySprite = std::make_unique<sf::Sprite>(*texture);
	RenderSpriteInRoom(window, entitySprite, targetSize, entitySnap.roomPos);
}

void DungeonView::InitSprites() {
    sp::AssetManager& assetManager = sp::ServiceLocator::GetAssetManager();

	auto loadSprite = [&](AssetId id, std::unique_ptr<sf::Sprite>& sprite) {
		if (!sprite) {
			auto texture = assetManager.GetAsset<sf::Texture>(id);
			assert(texture != nullptr);

			sprite = std::make_unique<sf::Sprite>(*texture);
			assert(sprite != nullptr);
		}
	};

	// get tile size
	s_tileSize = sf::Vector2f(assetManager.GetAsset<sf::Texture>(AssetId::BlockBackground)->getSize());

	loadSprite(AssetId::BlockBackground, m_tileBg);
	m_tileBg->setOrigin(s_tileSize / 2.f);

	loadSprite(AssetId::BlockTileset, m_tileSet);
	m_tileSet->setOrigin(s_tileSize / 2.f);
}

sf::IntRect DungeonView::GetTileTextureRect(DungeonTileType type) {
    // We cast the enum to an int to get the index (0 through 5)
    int index = static_cast<int>(type);
    
    // Calculate row and column
    int columns = 3; // Your tileset is 192px wide (192 / 64 = 3)
    int x = (index % columns) * (int)s_tileSize.x;
	int y = (index / columns) * (int)s_tileSize.y;

    return sf::IntRect({x, y}, sf::Vector2i(s_tileSize));
}

bool DungeonView::GetEntityTexture(EntityType entityType, std::shared_ptr<sf::Texture>& outTexture, sf::Vector2f& outTextureSize) {
	sp::AssetManager& assetManager = sp::ServiceLocator::GetAssetManager();

	switch (entityType) {
	case EntityType::Adventurer:
		outTexture = assetManager.GetAsset<sf::Texture>(AssetId::Adventurer);
		outTextureSize = s_tileSize * 0.5f * DUNGEON_SCALE_FACTOR;
		return true;
	default:
		return false;
	}
}
