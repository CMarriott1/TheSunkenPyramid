#include "bat.h"
#include "SFML/Graphics.hpp"
#include "constants.h"
#include "stdlib.h"
#include "math.h"

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
	const float hypot = sqrt((xdiff * xdiff) + (ydiff * ydiff));
	if (hypot != 0) {
		velocity.x = xdiff / hypot;
		velocity.y = ydiff / hypot;
	}
	else velocity = sf::Vector2f(0, 0);
	pos -= velocity * elapsed * GC::BatSpeed;
	spr.setPosition(pos);
}

int Bat::hurt() {
	--health;
	if (health <= 0) {
		active = false;
		return 1;
	}
	else return 0;
}

void Bat::activate(int playerPosition) {
	health = 3;
	if (playerPosition == GC::Up) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawn) + GC::MinSpawnDist , rand() % (GC::EnemySpawnHalf) + (GC::GameSize / 2) + GC::CharRadius + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Right) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawnHalf) + GC::MinSpawnDist, rand() % (GC::EnemySpawn) + GC::MinSpawnDist + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Down) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawn) + GC::MinSpawnDist, rand() % (GC::EnemySpawnHalf) + GC::MinSpawnDist + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Left) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawnHalf) + (GC::GameSize / 2) + (GC::CharRadius), rand() % (GC::EnemySpawn) + GC::MinSpawnDist + GC::LowerBounds.y));
	}
	active = true;
}

void Bat::render(sf::RenderWindow& window) {
	if (active) window.draw(spr);
}