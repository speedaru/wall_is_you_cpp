#pragma once
#include "pch.h"


class SharedGameState;

class Renderer {
public:
	void Render(sf::RenderWindow& window, sf::Clock& clock);
};

