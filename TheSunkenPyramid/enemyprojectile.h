#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct EnemyProjectile {
	sf::Sprite spr;
	bool active;
	sf::Vector2f velocity;

	void init(sf::Texture& tex);
	void update(float elapsedSec);
	void activate(const sf::Vector2f& enemyPosition, const sf::Vector2f& playerPosition);
	void render(sf::RenderWindow& window);
};