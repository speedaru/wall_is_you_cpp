#pragma once
#include "pch.h"

namespace sp {
	using EntityId = uint32_t;

	class Entity {
	public:
		Entity(EntityId id) : m_id(id) {}
		virtual ~Entity() = default;

		EntityId GetId() const { return m_id; }

	private:
		EntityId m_id;
	};
}
