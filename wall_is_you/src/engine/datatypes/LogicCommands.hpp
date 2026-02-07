#pragma once
#include "pch.h"

// payloads for different types of logic commands
struct LoadDungeonData {
    std::string path;
    int difficulty;
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

// different types of commands
enum class LogicCommandType {
    LoadDungeon,
    EntityInteraction,
    SettingsChange,
};


struct LogicCommand {
    LogicCommandType type;
    LogicPayload payload;
};