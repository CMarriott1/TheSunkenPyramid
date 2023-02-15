#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct Player {
	sf::Sprite spr;
	bool active;
	sf::Vector2f velocity;
	int fireDirection;
	void update(const sf::Vector2u& screenSize, float elapsedSec);
	void init(sf::Texture& tex);
};