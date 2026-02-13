#pragma once
#include "pch.h"

#include "datatypes/IDungeonEntity.hpp"
#include "datatypes/EntitySnapshot.hpp"


class EntitySystem {
public:
	EntitySystem& operator=(const EntitySystem& other) {
		m_entities.reserve(other.m_entities.size());

		// copy unique ptrs
		for (const auto& ent : other.m_entities) {
			m_entities.push_back(std::make_unique<IDungeonEntity>(*ent));
		}

		return *this;
	}

	template <typename... Args>
	void AddEntity(Args&&... args);

	EntitySystemSnapshot CreateSnapshot() const;

	// clears all entities
	void Clear();

	const std::vector<std::unique_ptr<IDungeonEntity>>& GetEntities() const;

	template <std::derived_from<IDungeonEntity> T>
	const T* GetEntity(sp::EntityId entId) const;

	sp::EntityId GetNewEntityId();

private:
	std::vector<std::unique_ptr<IDungeonEntity>> m_entities;
};


template <typename... Args>
void EntitySystem::AddEntity(Args&&... args) {
	m_entities.push_back(std::make_unique<IDungeonEntity>(std::forward<Args>(args)...));
}

template<std::derived_from<IDungeonEntity> T>
inline const T* EntitySystem::GetEntity(sp::EntityId entId) const {
	auto it = std::find_if(m_entities.begin(), m_entities.end(), [&](const auto& ent) {
		return ent->GetId() == entId;
	});

	if (it == m_entities.end()) {
		return nullptr;
	}

	return *it;
}
