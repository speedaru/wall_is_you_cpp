#pragma once
#include "pch.h"

#include "engine/datatypes/IView.hpp"

#include "game/datatypes/GameSnapshot.hpp"
#include "game/views/HudView.hpp"


class SharedGameState;
enum class DungeonTileType;
enum class AssetId;

// for entity visual registry map
struct EntityVisual {
	AssetId assetId;
	float scaleMultiplier; // relative to tile size
	sf::Vector2f pivotOffset; // to adjust if the sprite should stand on the bottom of the tile
	std::optional<int> randomTextureIdCount; // if texture is grid pick random idx between x cells
};

class DungeonView : public IView {
public:
	DungeonView() = delete;
	DungeonView(std::unique_ptr<SharedGameState>* sharedState);

	virtual bool HandleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
	virtual void Update(float dt) override;
	virtual void Render(sf::RenderWindow& window) override;

	virtual bool IsModal() const override { return true; }

private:
	// rendering
	void RenderSpriteInRoom(sf::RenderWindow& window, std::unique_ptr<sf::Sprite>& sprite, sf::Vector2f spriteSize, DungeonRoomPos roomPos) const;
	void RenderRoom(sf::RenderWindow& window, DungeonRoom* tileData, DungeonRoomPos roomPos);

	void SyncEntitySprites(const EntitySystemSnapshot& entitiesSnap);
	// create new entity sprite into m_entitySprites
	void CreateEntitySprite(const EntitySnapshot& entSnap, const EntityVisual& config);

	// load textures and stuff
	void InitTileSprites();

	// cellCount: non empty number of cells
	void SetRandomTextureFromGrid(const sf::Texture& texture, uint32_t cellCount, sf::Sprite& sprite);

private:
	std::unique_ptr<HudView> m_hudView;
	std::unique_ptr<SharedGameState>& m_sharedGameState;
	GameSnapshot m_gameSnap; // so if we can't pull snap, we render the last one

	// sprites
	std::unique_ptr<sf::Sprite> m_tileBg{};
	std::unique_ptr<sf::Sprite> m_tileSet{};
	std::map<uint32_t, sf::Sprite> m_entitySprites; // map entity id to a reusable sprite
};