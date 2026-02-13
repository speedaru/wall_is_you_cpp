#include "pch.h"
#include "DungeonView.hpp"

#include "engine/ServiceLocator.hpp"
#include "engine/WindowManager.hpp"
#include "engine/AssetManager.hpp"

#include "game/datatypes/UICommands.hpp"
#include "game/datatypes/SharedGameState.hpp"
//#include "game/datatypes/DungeonSnapshot.hpp"
#include "game/logic/dungeon_geom.hpp"
#include "game/constants.hpp"
#include "game/assets.hpp"

#include "utils/logging.hpp"
#include "utils/sf_events.hpp"
#include "utils/macros.hpp"
#include "utils/graphics.hpp"



static const std::unordered_map<EntityType, EntityVisual> ENTITY_VISUAL_REGISTRY = {
	{ EntityType::Adventurer,	{ AssetId::Adventurer,		0.6f, {0.f, 0.f}, std::nullopt }},
	{ EntityType::Dragon,		{ AssetId::Dragon,			0.5f, {0.f, 0.f}, std::nullopt } },
	{ EntityType::StrongSword,  { AssetId::StrongSword,		0.4f, {0.f, 0.f}, std::nullopt } },
	{ EntityType::ChaosSeal,	{ AssetId::ChaosSeal,		0.4f, {0.f, 0.f}, std::nullopt } },
	{ EntityType::Treasure,		{ AssetId::TreasuresGrid,	0.4f, {0.f, 0.f}, 5 } },
};


DungeonView::DungeonView(std::unique_ptr<SharedGameState>* sharedState)
	: m_hudView(std::make_unique<HudView>()), m_sharedGameState(*sharedState)
{
	InitTileSprites();
	dungeon_geom::InitTileSize(m_tileBg->getTexture());

	m_tileBg->setOrigin(dungeon_geom::g_tileSize / 2.f);
	m_tileSet->setOrigin(dungeon_geom::g_tileSize / 2.f);
}

bool DungeonView::HandleEvent(const sf::RenderWindow& window, const sf::Event& event) {
	m_hudView->HandleEvent(window, event);

	if (sp::utils::ui::IsKeyPressed(event, keybinds::EXIT_KEY)) {
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


void DungeonView::RenderSpriteInRoom(sf::RenderWindow& window, std::unique_ptr<sf::Sprite>& sprite, sf::Vector2f targetSpriteSize, DungeonRoomPos roomPos) const {
	// set sprite size to target size
	sp::utils::graphics::SetSpriteScale(*sprite, targetSpriteSize);

	// get room coord on screen
	sf::Vector2f roomPosOnScreen = dungeon_geom::GetRoomPosScreenCoords(m_gameSnap.dungeonSnap.layout, roomPos, AnchorType::Center);
	sprite->setPosition(roomPosOnScreen);
	window.draw(*sprite);
}

void DungeonView::RenderRoom(sf::RenderWindow& window, DungeonRoom* tileData, DungeonRoomPos roomPos) {
	// crop tilset to current tile
	sf::IntRect tile = sp::utils::graphics::GetTextureRectFromGrid(m_tileSet->getTexture(), dungeon_geom::g_tileSize, (int)tileData->type);
	m_tileSet->setTextureRect(tile);

	// rotate tile
	m_tileSet->setRotation(sf::degrees(tileData->rotations * 90.f));

	// render background + tile
	sf::Vector2f tileScaledSize = dungeon_geom::GetScaledTileSize();
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

        // create new sprite
        if (m_entitySprites.find(snap.id) == m_entitySprites.end()) {
			CreateEntitySprite(snap, config);
        }

        sf::Sprite& sprite = m_entitySprites.at(snap.id);

		// handle scaling, calculate scale based on the TILE_SIZE and the multiplier
		LOG_D("current texture width: %.2f\n", sprite.getLocalBounds().size.x);
		const sf::Vector2f scaledTileSize = dungeon_geom::GetScaledTileSize();
		const sf::Vector2f targetSize = scaledTileSize * config.scaleMultiplier;
		sp::utils::graphics::SetSpriteScale(sprite, targetSize);

		// set position
		sf::Vector2f renderPos = dungeon_geom::GetRoomPosScreenCoords(m_gameSnap.dungeonSnap.layout, snap.roomPos, AnchorType::Center);
		renderPos += config.pivotOffset; // apply offset
		sprite.setPosition(renderPos);
    }

	// delete entities that are not in entity system anymore
	std::erase_if(m_entitySprites, [&](const auto& pair) {
		return !activeIds.contains(pair.first);
	});
}

void DungeonView::CreateEntitySprite(const EntitySnapshot& entSnap, const EntityVisual& config) {
	auto texture = sp::ServiceLocator::GetAssetManager().GetAsset<sf::Texture>(config.assetId);
	m_entitySprites.emplace(entSnap.id, sf::Sprite(*texture));
	auto& sprite = m_entitySprites.at(entSnap.id);

	// select random texture
	if (config.randomTextureIdCount != std::nullopt) {
		SetRandomTextureFromGrid(*texture, config.randomTextureIdCount.value(), sprite);
	}

	// Set origin to center
	sf::Vector2u textureSize(sprite.getLocalBounds().size);
	sf::Vector2f textureCenter = dungeon_geom::AnchorCoords({ 0, 0 }, sf::Vector2f(textureSize), AnchorType::Center);
	m_entitySprites.at(entSnap.id).setOrigin(textureCenter);
	LOG_D("added sprite for ent id %u, type: %d, asset id: %d\n", entSnap.id, entSnap.type, config.assetId);
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

	loadSprite(AssetId::BlockBackground, m_tileBg);
	loadSprite(AssetId::BlocksGrid, m_tileSet);
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

	sf::Vector2f cellSize(cellLength, cellLength);
	sprite.setTextureRect(sp::utils::graphics::GetTextureRectFromGrid(texture, cellSize, randomIdx));
}

