#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct Jewel {
	sf::Sprite spr;
	int jewelType;
	bool found;

	void init(sf::Texture& tex, int Type);
	void collect(int index);
};