#include "pch.h"
#include "EntitySystem.hpp"


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
	return snap;
}

sp::EntityId EntitySystem::GetNewEntityId() {
	if (!m_entities.empty()) {
		return m_entities.back()->GetId() + 1;
	}
	return 0u;
}

