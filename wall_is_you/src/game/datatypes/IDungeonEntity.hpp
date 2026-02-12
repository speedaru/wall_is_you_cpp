#pragma once
#include "pch.h"

#include "engine/datatypes/Entity.hpp"

#include "DungeonLayout.hpp"
#include "EntityTypes.hpp"


class IDungeonEntity : public sp::Entity {
public:
	IDungeonEntity(sp::EntityId id, EntityType type, DungeonRoomPos pos)
		: Entity(id), m_type(type), m_roomPos(pos) {}

	EntityType GetType() const {
		return m_type;
	}

	DungeonRoomPos GetRoomPos() const {
		return m_roomPos;
	}

private:
	EntityType m_type;
	DungeonRoomPos m_roomPos;
};

