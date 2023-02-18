#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"

struct Mummy {
	sf::Sprite spr;
	bool active;
	sf::Vector2f velocity;
	int health;
	float timer;

	void init(sf::Texture& tex);
	bool update(float elapsed);
	int hurt();
	void activate(int playerPosition);
	void render(sf::RenderWindow&);
};