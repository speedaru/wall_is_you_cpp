#include "pch.h"
#include "DungeonModel.hpp"

void DungeonModel::GetSnapshot(DungeonSnapshot* outSnap) const {
	outSnap->layout = m_layout;
}

void DungeonModel::SetLayout(const DungeonLayout& layout) {
	m_layout = layout;
}

