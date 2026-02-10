#pragma once
#include "pch.h"

#include "engine/datatypes/Entity.hpp"
#include "DungeonLayout.hpp"
#include "EntityTypes.hpp"


struct EntitySnapshot {
	sp::EntityId id;
	DungeonRoomPos roomPos;
	EntityType type;
};

typedef std::vector<EntitySnapshot> EntitySystemSnapshot;
