#pragma once
#include "pch.h"
#include "DungeonLayout.hpp"
#include "EntitySnapshot.hpp"


struct DungeonSnapshot {
	DungeonLayout layout;
	EntitySystemSnapshot entitySystem;
};