#pragma once
#include "game/datatypes/DungeonEntity.hpp"
#include "game/datatypes/EntityTypes.hpp"


typedef std::vector<DungeonRoomPos> MovementPath;

class AdventurerEntity : public DungeonEntity {
public:
	AdventurerEntity(sp::EntityId id, DungeonRoomPos pos) : DungeonEntity(id, EntityType::Adventurer, pos) {}

private:
	MovementPath m_path;
};