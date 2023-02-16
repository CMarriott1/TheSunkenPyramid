#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct Bat {
	sf::Sprite spr;
	bool active;
	sf::Vector2f velocity;
	void init(sf::Texture& tex);
	void update(const sf::Vector2f& playerPosition, float elapsed);
	void activate(int playerPosition);
	void render(sf::RenderWindow&);
};