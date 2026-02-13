#include "pch.h"
#include "dungeon_loader.hpp"

#include "game/datatypes/entities/Adventurer.hpp"
#include "game/datatypes/entities/Dragon.hpp"
#include "game/datatypes/entities/StrongSwordEntity.hpp"
#include "game/datatypes/entities/ChaosSealEntity.hpp"
#include "game/datatypes/entities/TreasureEntity.hpp"
#include "DungeonModel.hpp"

#include "utils/logging.hpp"
#include "utils/io.hpp"


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


// function decl
static void ParseGrid(DungeonLayout& layout, std::stringstream& buffer);
static void ParseTags(EntitySystem& entitySystem, std::stringstream& buffer);

template <std::derived_from<IDungeonEntity> T>
static IDungeonEntity ParseEntity(EntitySystem& entitySystem, std::istringstream& ss);
template <std::derived_from<IDungeonEntity> T>
static IDungeonEntity ParseEntityWithLevel(EntitySystem& entitySystem, std::istringstream& ss);


void dungeon_loader::LoadFromFile(const fs::path& path, DungeonModel& dungeon) {
    // read file
    std::string rawContent = sp::utils::io::ReadFileStr(path);
    std::stringstream buffer(rawContent); // Standard stringstream

    DungeonLayout layout;
    ParseGrid(layout, buffer);

    EntitySystem entitySystem;
    ParseTags(entitySystem, buffer);

    dungeon.SetLayout(layout);
    dungeon.SetEntitySystem(entitySystem);
}


static void ParseGrid(DungeonLayout& layout, std::stringstream& buffer) {
    std::string line;
    if (std::getline(buffer, line)) {
        std::stringstream ss(line);
        if (!(ss >> layout.width >> layout.height)) 
            throw std::runtime_error("Invalid header format.");
    }

    layout.tiles.reserve((size_t)layout.width * layout.height);

    // Parse Grid
    for (uint32_t y = 0; y < layout.height; ++y) {
        if (!std::getline(buffer, line)) break;
        
        // Use SFML to handle the Unicode conversion safely
        sf::String sfLine = sf::String::fromUtf8(line.begin(), line.end());

        for (uint32_t x = 0; x < layout.width; ++x) {
            // Get the 32-bit unicode character
            uint32_t sym = (x < sfLine.getSize()) ? sfLine[x] : U' ';
            
            auto it = SYMBOL_LOOKUP.find(sym);
            if (it != SYMBOL_LOOKUP.end()) {
                layout.tiles.push_back(it->second);
            } else {
                layout.tiles.push_back({ DungeonTileType::Unknown, 0 });
            }
        }
    }
}

static void ParseTags(EntitySystem& entitySystem, std::stringstream& buffer) {
    std::string line;
    while (std::getline(buffer, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        
        std::string tag;
        ss >> tag;
        if (tag == "E") {
            std::string entityType;
            ss >> entityType;

            if (entityType == "A") 
                entitySystem.AddEntity(ParseEntityWithLevel<AdventurerEntity>(entitySystem, ss));
            else if (entityType == "D") 
                entitySystem.AddEntity(ParseEntityWithLevel<DragonEntity>(entitySystem, ss));
            else if (entityType == "S") 
                entitySystem.AddEntity(ParseEntity<StrongSwordEntity>(entitySystem, ss));
            else if (entityType == "CS") 
                entitySystem.AddEntity(ParseEntity<ChaosSealEntity>(entitySystem, ss));
            else if (entityType == "T") 
                entitySystem.AddEntity(ParseEntity<TreasureEntity>(entitySystem, ss));
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
