#include "pch.h"
#include "EntitySystem.hpp"

#include "utils/logging.hpp"


const std::vector<std::unique_ptr<IDungeonEntity>>& EntitySystem::GetEntities() const {
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
	LOG_D("snapped %llu entities\n", m_entities.size());
	return snap;
}

sp::EntityId EntitySystem::GetNewEntityId() {
	if (!m_entities.empty()) {
		return m_entities.back()->GetId() + 1;
	}
	return 0u;
}

