#include "pch.h"


enum class TurnPhase {
	Setup,				// Logic cleanup
	Input,				// wait for player to confirm input
	AdventurerPath,		// adventurer path calculation
	DragonMovement,		// dragons move randomly
};