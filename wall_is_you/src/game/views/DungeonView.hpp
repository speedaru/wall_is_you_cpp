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

	void InitSprites();
	sf::IntRect GetTextureRect(DungeonTileType type);

private:
	static inline constexpr int TILE_SIZE = 64;
	static inline constexpr float SCALE_FACTOR = 2.f;
	static inline constexpr sf::Vector2f TILE_CENTER = sf::Vector2f(TILE_SIZE / 2.f, TILE_SIZE / 2.f);
	static inline constexpr sf::Vector2f TILE_SCALED_SIZE = sf::Vector2f(TILE_SIZE * SCALE_FACTOR, TILE_SIZE * SCALE_FACTOR);

	std::unique_ptr<HudView> m_hudView;
	std::unique_ptr<SharedGameState>& m_sharedGameState;
	GameSnapshot m_gameSnap; // so if we can't pull snap, we render the last one

	// images
	std::unique_ptr<sf::Sprite> m_tileBg{};
	std::unique_ptr<sf::Sprite> m_tileSet{};
};