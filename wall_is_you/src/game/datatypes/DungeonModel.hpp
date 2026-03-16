#pragma once
#include "pch.h"
#include "game/datatypes/DungeonSnapshot.hpp"
#include "game/systems/EntitySystem.hpp"
	

class DungeonModel {
public:
	void GetSnapshot(DungeonSnapshot* outSnap) const;
	
	void SetLayout(const DungeonLayout& layout);
	void SetEntitySystem(const EntitySystem& entSystem);

	void ResetEntitySystem();

	void RotateRoom(const DungeonRoomPos& roomPos);

	constexpr bool IsLoaded() const {
		return !m_layout.tiles.empty();
	}

private:
	DungeonLayout m_layout;
	EntitySystem m_entitySystem;
};