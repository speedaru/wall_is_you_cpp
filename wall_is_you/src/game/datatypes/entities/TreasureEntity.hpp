#pragma once
#include "game/datatypes/IDungeonEntity.hpp"


class TreasureEntity : public IDungeonEntity {
public:
	TreasureEntity(sp::EntityId id, DungeonRoomPos pos)
		: IDungeonEntity(id, EntityType::Treasure, pos)
	{
	}
};
