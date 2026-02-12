#pragma once
#include "game/datatypes/IDungeonEntity.hpp"


class StrongSwordEntity : public IDungeonEntity {
public:
	StrongSwordEntity(sp::EntityId id, DungeonRoomPos pos)
		: IDungeonEntity(id, EntityType::Dragon, pos)
	{
	}
};
