#pragma once
#include "game/datatypes/IDungeonEntity.hpp"


class ChaosSealEntity : public IDungeonEntity {
public:
	ChaosSealEntity(sp::EntityId id, DungeonRoomPos pos)
		: IDungeonEntity(id, EntityType::ChaosSeal, pos)
	{
	}
};
