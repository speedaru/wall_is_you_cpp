#include "pch.h"
#include "DungeonModel.hpp"

void DungeonModel::GetSnapshot(DungeonSnapshot* outSnap) const {
	outSnap->layout = m_layout;
}

void DungeonModel::SetDimensions() {
	m_layout.width = 1;
	m_layout.height = 1;
}
