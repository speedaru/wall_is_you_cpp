#pragma once
#include "game/datatypes/IDungeonEntity.hpp"


class DragonEntity : public IDungeonEntity {
public:
	DragonEntity(sp::EntityId id, DungeonRoomPos pos, uint32_t level)
		: IDungeonEntity(id, EntityType::Dragon, pos), m_level(level)
	{
	}

private:
	uint32_t m_level;
};
