#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct Mummy {
	sf::Sprite spr;
	bool active;
	sf::Vector2f velocity;
	int health;

	void init(sf::Texture& tex);
	void update(const sf::Vector2f& playerPosition, float elapsed);
	int hurt();
	void activate(int playerPosition);
	void render(sf::RenderWindow&);
};