#include "pch.h"
#include "EntitySystem.hpp"


void EntitySystem::AddEntity(std::unique_ptr<DungeonEntity>&& entity) {
	m_entities.push_back(std::move(entity));
}

const std::vector<std::unique_ptr<DungeonEntity>>& EntitySystem::GetEntities() const {
	return m_entities;
}

EntitySystemSnapshot EntitySystem::CreateSnapshot() const {
	EntitySystemSnapshot snap;
	for (const auto& ent : m_entities) {
		EntitySnapshot entSnap;
		entSnap.id = ent->GetId();
		entSnap.type = ent->GetType();
		entSnap.roomPos = ent->GetRoomPos();
		snap.push_back(entSnap);
	}
}

