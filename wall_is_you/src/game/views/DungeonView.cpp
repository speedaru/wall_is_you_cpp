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


struct EntityVisual {
	AssetId assetId;
	float scaleMultiplier; // relative to tile size
	sf::Vector2f pivotOffset; // to adjust if the sprite should stand on the bottom of the tile
};

static const std::unordered_map<EntityType, EntityVisual> ENTITY_VISUAL_REGISTRY = {
	{ EntityType::Adventurer,	{ AssetId::Adventurer,		0.6f, {0.f, 0.f} } },
	{ EntityType::Dragon,		{ AssetId::Dragon,			0.5f, {0.f, 0.f} } },
	{ EntityType::StrongSword,  { AssetId::StrongSword,		0.4f, {0.f, 0.f} } },
	{ EntityType::ChaosSeal,	{ AssetId::ChaosSeal,		0.5f, {0.f, 0.f} } },
	{ EntityType::Treasure,		{ AssetId::TreasuresGrid,	0.4f, {0.f, 0.f} } },
};


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

	// get new game snapshot to render
	if (m_sharedGameState->PullGameSnap(m_gameSnap)) {
		SyncEntitySprites(m_gameSnap.dungeonSnap.entitySystem);
	}
}

void DungeonView::Render(sf::RenderWindow& window) {
	// render dungeon layout
    for (DungeonLayoutIterator it = m_gameSnap.dungeonSnap.layout; !it.Finished(); it++) {
		RenderRoom(window, *it, it.GetRoomPos());
    }

	// render entities
	for (auto& [id, sprite] : m_entitySprites) {
		window.draw(sprite);
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
	m_tileSet->setTextureRect(GetTextureRect(m_tileSet->getTexture(), sf::Vector2u(s_tileSize), (int)tileData->type));

	// rotate tile
	m_tileSet->setRotation(sf::degrees(tileData->rotations * 90.f));

	// render background + tile
	sf::Vector2f tileScaledSize = s_tileSize * DUNGEON_SCALE_FACTOR;
	RenderSpriteInRoom(window, m_tileBg, tileScaledSize, roomPos);
	RenderSpriteInRoom(window, m_tileSet, tileScaledSize, roomPos);
}

void DungeonView::SyncEntitySprites(const EntitySystemSnapshot& entitiesSnap) {
	std::set<uint32_t> activeIds;

    for (const auto& snap : entitiesSnap) {
        activeIds.insert(snap.id);
        
        // set visual config
        auto visualIt = ENTITY_VISUAL_REGISTRY.find(snap.type);
        if (visualIt == ENTITY_VISUAL_REGISTRY.end()) continue;
        const auto& config = visualIt->second;

        // get or set sprite
        if (m_entitySprites.find(snap.id) == m_entitySprites.end()) {
            auto texture = sp::ServiceLocator::GetAssetManager().GetAsset<sf::Texture>(config.assetId);
			m_entitySprites.emplace(snap.id, sf::Sprite(*texture));
			auto& sprite = m_entitySprites.at(snap.id);

			// if treasure select random texture
			if (snap.type == EntityType::Treasure) {
				SetRandomTextureFromGrid(*texture, 5, sprite);
			}
            
            // Set origin to bottom-center or center depending on your art style
            sf::Vector2u texSize(sprite.getLocalBounds().size);
            m_entitySprites.at(snap.id).setOrigin({ texSize.x / 2.f, texSize.y / 2.f });
			LOG_D("added sprite for ent id %u, type: %d, asset id: %d\n", snap.id, snap.type, config.assetId);
        }

        sf::Sprite& sprite = m_entitySprites.at(snap.id);

		// handle scaling
        // calculate scale based on the TILE_SIZE and the multiplier
        float targetWidth = s_tileSize.x * DUNGEON_SCALE_FACTOR * config.scaleMultiplier;
        float currentTexWidth = static_cast<float>(sprite.getLocalBounds().size.x);
		LOG_D("current texture width: %.2f\n", currentTexWidth);
        float finalScale = targetWidth / currentTexWidth;
        sprite.setScale({ finalScale, finalScale });

        // position with offset
        float posX = (snap.roomPos.col * s_tileSize.x * DUNGEON_SCALE_FACTOR) + (s_tileSize.x * DUNGEON_SCALE_FACTOR / 2.f);
        float posY = (snap.roomPos.row * s_tileSize.y * DUNGEON_SCALE_FACTOR) + (s_tileSize.y * DUNGEON_SCALE_FACTOR / 2.f);
        
        // apply the pivot offset from config
        sprite.setPosition({ posX + config.pivotOffset.x, posY + config.pivotOffset.y });
    }

	std::erase_if(m_entitySprites, [&](const auto& pair) {
		return !activeIds.contains(pair.first);
	});
}

void DungeonView::InitTileSprites() {
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

	loadSprite(AssetId::BlocksGrid, m_tileSet);
	m_tileSet->setOrigin(s_tileSize / 2.f);
}

sf::IntRect DungeonView::GetTextureRect(const sf::Texture& textureGrid, sf::Vector2u cellSize, int textureIdx) {
    int columns = textureGrid.getSize().x / cellSize.x;

    int x = (textureIdx % columns) * cellSize.x;
	int y = (textureIdx / columns) * cellSize.y;

    return sf::IntRect({x, y}, sf::Vector2i(cellSize));
}

void DungeonView::SetRandomTextureFromGrid(const sf::Texture& texture, uint32_t cellCount, sf::Sprite& sprite) {
	std::random_device random;
	std::mt19937 rng(random());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, cellCount - 1);

	uint32_t randomIdx = dist(rng);
	
	sf::Vector2u textureSize = texture.getSize();
	float area = static_cast<float>(textureSize.x * textureSize.y);

	// if not even number of cells, then add 1 to get total number of cols
	float totalGridCols = static_cast<float>(cellCount);
	if (cellCount & 1) { // if odd
		totalGridCols++;
	}

	float cellLength = sqrtf(area / totalGridCols);
	if (cellLength != (uint32_t)cellLength) {
		throw new std::exception("failed to find cellSize");
	}

	sf::Vector2u cellSize((uint32_t)cellLength, (uint32_t)cellLength);
	sprite.setTextureRect(GetTextureRect(texture, cellSize, randomIdx));
}

