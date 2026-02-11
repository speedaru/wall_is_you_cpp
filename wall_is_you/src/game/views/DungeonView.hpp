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
		InitSprites();
	}

	virtual bool HandleEvent(const sf::RenderWindow& window, const sf::Event& event) override;
	virtual void Update(float dt) override;
	virtual void Render(sf::RenderWindow& window) override;

	virtual bool IsModal() const override { return true; }

private:
	// rendering
	void RenderSpriteInRoom(sf::RenderWindow& window, std::unique_ptr<sf::Sprite>& sprite, sf::Vector2f spriteSize, DungeonRoomPos roomPos);
	void RenderRoom(sf::RenderWindow& window, DungeonRoom* tileData, DungeonRoomPos roomPos);
	void RenderEntity(sf::RenderWindow& window, const EntitySnapshot& entitySnap);

	void InitSprites();
	sf::IntRect GetTileTextureRect(DungeonTileType type);
	bool GetEntityTexture(EntityType entityType, std::shared_ptr<sf::Texture>& outTexture, sf::Vector2f& outTextureSize);

private:
	static inline constexpr float DUNGEON_SCALE_FACTOR = 2.f;
	static inline sf::Vector2f s_tileSize;

	std::unique_ptr<HudView> m_hudView;
	std::unique_ptr<SharedGameState>& m_sharedGameState;
	GameSnapshot m_gameSnap; // so if we can't pull snap, we render the last one

	// images
	std::unique_ptr<sf::Sprite> m_tileBg{};
	std::unique_ptr<sf::Sprite> m_tileSet{};
};