#pragma once
#include "game/datatypes/IDungeonEntity.hpp"
#include "game/datatypes/EntityTypes.hpp"


typedef std::vector<DungeonRoomPos> MovementPath;

class AdventurerEntity : public IDungeonEntity {
public:
	AdventurerEntity(sp::EntityId id, DungeonRoomPos pos, uint32_t level)
		: IDungeonEntity(id, EntityType::Adventurer, pos), m_level(level)
	{

	}

private:
	MovementPath m_path;
	uint32_t m_level;
};