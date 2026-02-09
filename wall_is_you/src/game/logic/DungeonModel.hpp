#pragma once
#include "pch.h"
#include "game/datatypes/DungeonSnapshot.hpp"
	

class DungeonModel {
public:

	void GetSnapshot(DungeonSnapshot* outSnap) const;
	
	void SetLayout(const DungeonLayout& layout);

private:
	DungeonLayout m_layout;
};