#pragma once
#include "pch.h"

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

using LogicPayload = std::variant<std::monostate,
    LoadDungeonData,
    EntityInteractionData,
    SettingsChangeData
>;


struct LogicCommand {
	enum class Type {
		LoadDungeon,
		EntityInteraction,
		SettingsChange,
	} type;
    LogicPayload payload;
};