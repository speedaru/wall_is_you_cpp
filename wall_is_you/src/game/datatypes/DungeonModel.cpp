#include "pch.h"
#include "DungeonModel.hpp"

void DungeonModel::GetSnapshot(DungeonSnapshot* outSnap) const {
	outSnap->layout = m_layout;
	outSnap->entitySystem = m_entitySystem.CreateSnapshot();
}

void DungeonModel::SetLayout(const DungeonLayout& layout) {
	m_layout = layout;
}

void DungeonModel::SetEntitySystem(const EntitySystem& entSystem) {
	m_entitySystem = entSystem;
}

void DungeonModel::ResetEntitySystem() {
	m_entitySystem.Clear();
}

void DungeonModel::RotateRoom(const DungeonRoomPos& roomPos) {
	assert(roomPos.row < m_layout.height && roomPos.col < m_layout.width);
	DungeonRoom& room = m_layout.GetRoom(roomPos);
	room.rotations = (room.rotations + 1) % 4; // keep at 0, 1, 2, 3
}

