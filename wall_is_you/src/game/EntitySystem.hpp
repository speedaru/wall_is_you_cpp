#pragma once
#include "pch.h"

#include "datatypes/DungeonEntity.hpp"
#include "datatypes/EntitySnapshot.hpp"


class EntitySystem {
public:
	//EntitySystem& operator=(const EntitySystem& other) {
	//	m_entities.reserve(other.m_entities.size());

	//	// copy unique ptrs
	//	for (const auto& ent : m_entities) {
	//		m_entities.push_back(std::make_unique<DungeonEntity>(*ent));
	//	}

	//	return *this;
	//}

	void AddEntity(std::unique_ptr<DungeonEntity>&& entity);

	const std::vector<std::unique_ptr<DungeonEntity>>& GetEntities() const;

	EntitySystemSnapshot CreateSnapshot() const;

private:
	std::vector<std::unique_ptr<DungeonEntity>> m_entities;
};