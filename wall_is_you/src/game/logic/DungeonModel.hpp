#pragma once
#include "pch.h"
#include "game/datatypes/DungeonSnapshot.hpp"
#include "game/EntitySystem.hpp"
	

class DungeonModel {
public:
	void GetSnapshot(DungeonSnapshot* outSnap) const;
	
	void SetLayout(const DungeonLayout& layout);

private:
	DungeonLayout m_layout;
	EntitySystem m_entitySystem;
};