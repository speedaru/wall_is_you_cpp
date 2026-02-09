#include "pch.h"
#include "dungeon_loader.hpp"
#include <fstream>
#include <codecvt>

#include "DungeonModel.hpp"

#include "utils/logging.hpp"

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


// private funcitons
// Helper to convert UTF-8 string to UTF-32 string
static std::u32string ToUTF32(const std::string& str) {
    try {
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        return convert.from_bytes(str);
    } catch (...) {
        throw std::runtime_error("Failed to convert file to UTF-32. Ensure file is UTF-8 encoded.");
    }
}


void dungeon_loader::LoadFromFile(const fs::path& path, DungeonModel& dungeon) {
	// 1. Read raw file
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("File not found: " + path.string());
    
    std::string rawContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::u32string u32Content = ToUTF32(rawContent);
    
    // 2. Use a U32 stream
    std::basic_stringstream<char32_t> buffer(u32Content);
    std::u32string line;
    DungeonLayout layout;

    // Header (Width Height)
    if (std::getline(buffer, line)) {
        // Convert U32 line back to string just for the width/height numbers
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        std::stringstream ss(convert.to_bytes(line));
        if (!(ss >> layout.width >> layout.height)) 
            throw std::runtime_error("Invalid header format.");
    }

    layout.tiles.reserve((size_t)layout.width * layout.height);

    // 3. Parse Grid
    for (uint32_t y = 0; y < layout.height; ++y) {
        if (!std::getline(buffer, line)) break;
        
        // Clean up Windows \r if present
        if (!line.empty() && line.back() == U'\r') line.pop_back();

        for (uint32_t x = 0; x < layout.width; ++x) {
            char32_t sym = (x < line.length()) ? line[x] : U' ';
            
            auto it = SYMBOL_LOOKUP.find(sym);
            if (it != SYMBOL_LOOKUP.end()) {
                LOG_D("pushing tile in room: %u %u\n", x, y);
                layout.tiles.push_back(it->second);
            } else {
                layout.tiles.push_back({ DungeonTileType::Unknown, 0 });
            }
        }
    }

    // 4. Parse Tags (E, GR)
    while (std::getline(buffer, line)) {
        if (line.empty()) continue;
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
        std::stringstream ss(convert.to_bytes(line));
        
        std::string tag;
        ss >> tag;
        // ... Handle tag parsing as before ...
    }

    // FIXING YOUR DEBUG LOG: i * layout.width + j
    printf("tile type:\n");
    for (size_t i = 0; i < layout.height; i++) {
        for (size_t j = 0; j < layout.width; j++) {
            printf("%u ", layout.tiles[i * layout.width + j].type);
        }
        printf("\n");
    }

    dungeon.SetLayout(layout);
}
