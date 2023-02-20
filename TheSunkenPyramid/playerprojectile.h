#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct PlayerProjectile {
	sf::Sprite spr;
	bool active;
	sf::Vector2f velocity;
	bool loop;

	void init(sf::Texture& tex);
	void update(float elapsedSec);
	void activate(int direction, const sf::Vector2f& playerPosition, std::vector<int>offset, bool screenLoop);
	void render(sf::RenderWindow& window);
};