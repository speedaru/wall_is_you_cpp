#pragma once
#include "pch.h"

struct SpImage {
	sf::Texture texture;
	sf::Sprite sprite;

	SpImage(sf::Texture& texture) : texture(std::move(texture)), sprite(this->texture) { }
};