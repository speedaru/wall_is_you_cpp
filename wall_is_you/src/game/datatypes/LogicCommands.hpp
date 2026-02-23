#pragma once
#include "pch.h"
#include "game/datatypes/DungeonLayout.hpp"

// payloads for different types of logic commands
struct LoadDungeonData {
    fs::path path;
};

struct EntityInteractionData {
    int entityId;
    int actionType;
};

struct SettingsChangeData {
    float volume;
    bool permadeath;
};

struct RotateRoomData {
    DungeonRoomPos roomPos;
};

using LogicPayload = std::variant<std::monostate,
    LoadDungeonData,
    EntityInteractionData,
    SettingsChangeData,
    RotateRoomData
>;


struct LogicCommand {
	enum class Type {
		HandleLoadDungeon,
		EntityInteraction,
		SettingsChange,
        RotateRoom
	} type;
    LogicPayload payload;
};