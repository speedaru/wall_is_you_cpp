#pragma once
#include "pch.h"

#include "engine/datatypes/IView.hpp"

#include "game/datatypes/GameSnapshot.hpp"
#include "game/views/HudView.hpp"


class SharedGameState;
enum class DungeonTileType;

class DungeonView : public IView {
public:
	DungeonView() = delete;
	DungeonView(std::unique_ptr<SharedGameState>* sharedState)
		: m_hudView(std::make_unique<HudView>()), m_sharedGameState(*sharedState)
	{
		InitTileSprites();
	}

	virtual bool HandleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
	virtual void Update(float dt) override;
	virtual void Render(sf::RenderWindow& window) override;

	virtual bool IsModal() const override { return true; }

private:
	// rendering
	void RenderSpriteInRoom(sf::RenderWindow& window, std::unique_ptr<sf::Sprite>& sprite, sf::Vector2f spriteSize, DungeonRoomPos roomPos);
	void RenderRoom(sf::RenderWindow& window, DungeonRoom* tileData, DungeonRoomPos roomPos);

	void SyncEntitySprites(const EntitySystemSnapshot& entitiesSnap);

	// load textures and stuff
	void InitTileSprites();

	// texture transformation and calculations
	sf::IntRect GetTextureRect(const sf::Texture& textureGrid, sf::Vector2u cellSize, int textureIdx);

	// cellCount: non empty number of cells
	void SetRandomTextureFromGrid(const sf::Texture& texture, uint32_t cellCount, sf::Sprite& sprite);

private:
	static inline constexpr float DUNGEON_SCALE_FACTOR = 2.f;
	static inline sf::Vector2f s_tileSize;

	std::unique_ptr<HudView> m_hudView;
	std::unique_ptr<SharedGameState>& m_sharedGameState;
	GameSnapshot m_gameSnap; // so if we can't pull snap, we render the last one

	// sprites
	std::unique_ptr<sf::Sprite> m_tileBg{};
	std::unique_ptr<sf::Sprite> m_tileSet{};
	std::map<uint32_t, sf::Sprite> m_entitySprites; // map entity id to a reusable sprite
};