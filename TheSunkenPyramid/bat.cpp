#include "bat.h"
#include "SFML/Graphics.hpp"
#include "constants.h"
#include "stdlib.h"

void Bat::init(sf::Texture& tex) {
	active = false;
	spr.setTexture(tex);
	sf::IntRect texR = spr.getTextureRect();
	spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
}

void Bat::update(const sf::Vector2f& playerPosition, float elapsed) {
	sf::Vector2f pos = spr.getPosition();
	const float xdiff = pos.x - playerPosition.x;
	const float ydiff = pos.y - playerPosition.y;
	const float positionDifference = abs(xdiff) + abs(ydiff);
	velocity.x = xdiff / positionDifference * GC::BatSpeed;
	velocity.y = ydiff / positionDifference * GC::BatSpeed;
	pos += velocity;
	spr.setPosition(pos);
}

void Bat::activate(int playerPosition) {
	if (playerPosition == GC::Up) {
		spr.setPosition(sf::Vector2f(rand() % GC::GameSize + 1, rand() % (GC::GameSize / 2) + 1 + (GC::GameSize / 2) + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Right) {
		spr.setPosition(sf::Vector2f(rand() % (GC::GameSize / 2) + 1, rand() % GC::GameSize + 1 + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Down) {
		spr.setPosition(sf::Vector2f(rand() % GC::GameSize + 1, rand() % (GC::GameSize / 2) + 1 + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Left) {
		spr.setPosition(sf::Vector2f(rand() % (GC::GameSize/2) + 1 + (GC::GameSize / 2), rand() % GC::GameSize + 1 + GC::LowerBounds.y));
	}
	active = true;
}

void Bat::render(sf::RenderWindow& window) {
	if (active) window.draw(spr);
}