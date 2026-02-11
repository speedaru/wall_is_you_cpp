#pragma once
#include "game/datatypes/DungeonEntity.hpp"


class DragonEntity : public DungeonEntity {
public:
	DragonEntity(sp::EntityId id, DungeonRoomPos pos) : DungeonEntity(id, EntityType::Dragon, pos) {}
};
