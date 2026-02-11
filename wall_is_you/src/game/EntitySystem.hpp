#pragma once
#include "pch.h"

#include "datatypes/DungeonEntity.hpp"
#include "datatypes/EntitySnapshot.hpp"


class EntitySystem {
public:
	EntitySystem& operator=(const EntitySystem& other) {
		m_entities.reserve(other.m_entities.size());

		// copy unique ptrs
		for (const auto& ent : other.m_entities) {
			m_entities.push_back(std::make_unique<DungeonEntity>(*ent));
		}

		return *this;
	}

	template <typename... Args>
	void AddEntity(Args&&... args);

	EntitySystemSnapshot CreateSnapshot() const;

	const std::vector<std::unique_ptr<DungeonEntity>>& GetEntities() const;

	sp::EntityId GetNewEntityId();

private:
	std::vector<std::unique_ptr<DungeonEntity>> m_entities;
};


template <typename... Args>
void EntitySystem::AddEntity(Args&&... args) {
	m_entities.push_back(std::make_unique<DungeonEntity>(std::forward<Args>(args)...));
}
