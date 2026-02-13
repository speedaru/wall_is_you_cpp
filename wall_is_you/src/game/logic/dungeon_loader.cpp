#include "pch.h"
#include "dungeon_loader.hpp"
#include <fstream>
#include <codecvt>

#include "game/datatypes/entities/Adventurer.hpp"
#include "game/datatypes/entities/Dragon.hpp"
#include "game/datatypes/entities/StrongSwordEntity.hpp"
#include "game/datatypes/entities/ChaosSealEntity.hpp"
#include "game/datatypes/entities/TreasureEntity.hpp"
#include "DungeonModel.hpp"

#include "utils/logging.hpp"
#include "utils/io.hpp"

#pragma warning(disable: 4566) // encoding


static const std::unordered_map<char32_t, DungeonRoom> SYMBOL_LOOKUP = {
	{U'□', {DungeonTileType::Solid, 0}},
    {U'╨', {DungeonTileType::Single, 0}}, {U'╞', {DungeonTileType::Single, 1}},
    {U'╥', {DungeonTileType::Single, 2}}, {U'╡', {DungeonTileType::Single, 3}},
    {U'╚', {DungeonTileType::DoubleAdj, 0}}, {U'╔', {DungeonTileType::DoubleAdj, 1}},
    {U'╗', {DungeonTileType::DoubleAdj, 2}}, {U'╝', {DungeonTileType::DoubleAdj, 3}},
    {U'║', {DungeonTileType::DoubleOpp, 0}}, {U'═', {DungeonTileType::DoubleOpp, 1}},
    {U'╠', {DungeonTileType::Triple, 0}}, {U'╦', {DungeonTileType::Triple, 1}},
    {U'╣', {DungeonTileType::Triple, 2}}, {U'╩', {DungeonTileType::Triple, 3}},
    {U'╬', {DungeonTileType::Quad, 0}},
    {U' ', {DungeonTileType::Unknown, 0}}
};


// Helper to convert UTF-8 string to UTF-32 string
static std::u32string ToUTF32(const std::string& str) {
    try {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        return convert.from_bytes(str);
    } catch (...) {
        throw std::runtime_error("Failed to convert file to UTF-32. Ensure file is UTF-8 encoded.");
    }
}

// function decl
static void ParseGrid(DungeonLayout& layout, std::basic_stringstream<char32_t>& buffer);
static void ParseTags(EntitySystem& entitySystem, std::basic_stringstream<char32_t>& buffer);


void dungeon_loader::LoadFromFile(const fs::path& path, DungeonModel& dungeon) {
    // read file
    std::string rawContent = sp::utils::io::ReadFileStr(path);
    std::u32string u32Content = ToUTF32(rawContent);
    std::basic_stringstream<char32_t> buffer(u32Content); // Use a U32 stream

    DungeonLayout layout;
    ParseGrid(layout, buffer);

    EntitySystem entitySystem;
    ParseTags(entitySystem, buffer);

    dungeon.SetLayout(layout);
    dungeon.SetEntitySystem(entitySystem);
}


static void ParseGrid(DungeonLayout& layout, std::basic_stringstream<char32_t>& buffer) {
    std::u32string line;

    // Header (Width Height)
    if (std::getline(buffer, line)) {
        // Convert U32 line back to string just for the width/height numbers
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        std::stringstream ss(convert.to_bytes(line));
        if (!(ss >> layout.width >> layout.height)) 
            throw std::runtime_error("Invalid header format.");
    }

    layout.tiles.reserve((size_t)layout.width * layout.height);

    // Parse Grid
    for (uint32_t y = 0; y < layout.height; ++y) {
        if (!std::getline(buffer, line)) break;
        
        // Clean up Windows \r if present
        if (!line.empty() && line.back() == U'\r') line.pop_back();

        for (uint32_t x = 0; x < layout.width; ++x) {
            char32_t sym = (x < line.length()) ? line[x] : U' ';
            
            auto it = SYMBOL_LOOKUP.find(sym);
            if (it != SYMBOL_LOOKUP.end()) {
                layout.tiles.push_back(it->second);
            } else {
                layout.tiles.push_back({ DungeonTileType::Unknown, 0 });
            }
        }
    }
}

// basic entity derived from IDungeonEntity
template <std::derived_from<IDungeonEntity> T>
static IDungeonEntity ParseEntity(EntitySystem& entitySystem, std::istringstream& ss) {
    DungeonRoomPos roomPos;
    ss >> roomPos.row;
    ss >> roomPos.col;

    return T(entitySystem.GetNewEntityId(), roomPos);
}

// entity derived from IDungeonEntity that has a level
template <std::derived_from<IDungeonEntity> T>
static IDungeonEntity ParseEntityWithLevel(EntitySystem& entitySystem, std::istringstream& ss) {
    DungeonRoomPos roomPos;
    ss >> roomPos.row;
    ss >> roomPos.col;

    uint32_t level;
    ss >> level;

    return T(entitySystem.GetNewEntityId(), roomPos, level);
}

static void ParseTags(EntitySystem& entitySystem, std::basic_stringstream<char32_t>& buffer) {
	std::u32string line;

    while (std::getline(buffer, line)) {
        if (line.empty()) continue;
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        std::istringstream ss(convert.to_bytes(line));
        
        std::string tag;
        ss >> tag;

        if (tag == "E") {
            std::string entityType;
            ss >> entityType;

			if (entityType == "A") {
				entitySystem.AddEntity(ParseEntityWithLevel<AdventurerEntity>(entitySystem, ss));
                printf("parsed adventurer, ent id: %u\n", entitySystem.GetEntities().back()->GetId());
			}
			else if (entityType == "D") {
				entitySystem.AddEntity(ParseEntityWithLevel<DragonEntity>(entitySystem, ss));
                printf("parsed dragon, ent id: %u\n", entitySystem.GetEntities().back()->GetId());
			}
			else if (entityType == "S") {
				entitySystem.AddEntity(ParseEntity<StrongSwordEntity>(entitySystem, ss));
                printf("parsed strong sword, ent id: %u\n", entitySystem.GetEntities().back()->GetId());
			}
			else if (entityType == "CS") {
				entitySystem.AddEntity(ParseEntity<ChaosSealEntity>(entitySystem, ss));
                printf("parsed chaos seal, ent id: %u\n", entitySystem.GetEntities().back()->GetId());
			}
			else if (entityType == "T") {
				entitySystem.AddEntity(ParseEntity<TreasureEntity>(entitySystem, ss));
                printf("parsed treasure, ent id: %u\n", entitySystem.GetEntities().back()->GetId());
			}
        }
    }
}
