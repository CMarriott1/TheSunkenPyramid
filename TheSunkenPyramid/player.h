#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct Player {
	void update(const sf::Vector2u& screenSize, float elapsedSec);
	void init(sf::Texture& tex);

};