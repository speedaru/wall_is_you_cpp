#pragma once
#include "pch.h"


enum class DungeonTileType {
	Solid,
	Single,
	DoubleAdj,
	DoubleOpp,
	Triple,
	Quad,
	Unknown = 0xffff,
};

struct DungeonRoom {
	DungeonTileType type;
	uint32_t rotations{ 0 };
};

struct DungeonRoomPos {
	uint32_t row{};
	uint32_t col{};
};

// row major
struct DungeonLayout {
    uint32_t width;
    uint32_t height;
    std::vector<DungeonRoom> tiles; // Flat vector representing the grid (index = y * width + x)
};

class DungeonLayoutIterator {
public:
	// init ptr to first, or nullptr if empty
	DungeonLayoutIterator(DungeonLayout& layout)
		: m_layout(layout), m_ptr(layout.tiles.empty() ? nullptr : &layout.tiles.front()) { }

	void Begin() {
		m_ptr = &m_layout.tiles.front();
	}

	void End() {
		m_ptr = &m_layout.tiles.back();
	}

	void Next() {
		DungeonRoom* Next = m_ptr + 1;
		if (Next <= &m_layout.tiles.back()) {
			m_ptr = Next;
		}
		else {
			m_ptr = nullptr;
		}
	}

	bool Finished() {
		return m_ptr == nullptr;
	}

	DungeonRoom* Get() const {
		return m_ptr;
	}

	DungeonRoomPos GetRoomPos() const {
		uint32_t i = static_cast<uint32_t>(m_ptr - &m_layout.tiles.front());
		DungeonRoomPos roomPos;
		roomPos.row = i / m_layout.height;
		roomPos.col = i % m_layout.height;
		return roomPos;
	}

	DungeonLayoutIterator operator++(int) {
		DungeonLayoutIterator old = *this;
		Next();
		return old;
	}

	DungeonRoom* operator*() const {
		return m_ptr;
	}

private:
	DungeonLayout& m_layout;
	DungeonRoom* m_ptr;
};
