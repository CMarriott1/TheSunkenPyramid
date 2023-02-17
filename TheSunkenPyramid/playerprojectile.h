#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct PlayerProjectile {
	sf::Sprite spr;
	bool active;
	sf::Vector2f velocity;

	void init(sf::Texture& tex);
	void update(float elapsedSec);
	void activate(int direction, const sf::Vector2f& playerPosition);
	void render(sf::RenderWindow& window);
};