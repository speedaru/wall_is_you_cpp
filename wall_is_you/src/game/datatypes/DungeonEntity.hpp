#pragma once
#include "pch.h"

#include "engine/datatypes/Entity.hpp"

#include "DungeonLayout.hpp"
#include "EntityTypes.hpp"


class DungeonEntity : public sp::Entity {
public:
	//DungeonEntity(sp::EntityId id) : Entity(id) {}

	DungeonEntity(sp::EntityId id, EntityType type, DungeonRoomPos pos)
		: Entity(id), m_type(type), m_roomPos(pos) {}

	//void SetType(EntityType type) {
	//	m_type = type;
	//}

	//void SetRoomPos(DungeonRoomPos pos) {
	//	m_roomPos = pos;
	//}

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

