#include "mummy.h"
#include "SFML/Graphics.hpp"
#include "constants.h"

void Mummy::init(sf::Texture& tex) {
	active = false;
	spr.setTexture(tex);
	sf::IntRect texR = spr.getTextureRect();
	spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
}

bool Mummy::update(float elapsed) {
	timer -= elapsed;
	if (timer <= 0)
	{
		timer = GC::EnemyFiringSpeed;
		return true;
	}
	else return false;
}

int Mummy::hurt() {
	--health;
	if (health <= 0) {
		active = false;
		return 1;
	}
	else return 0;
}

void Mummy::activate(int playerPosition) {
	health = 3;
	timer = 1;
	if (playerPosition == GC::Up) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawn)+GC::MinSpawnDist, rand() % (GC::EnemySpawnHalf)+(GC::GameSize / 2) + GC::CharRadius + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Right) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawnHalf)+GC::MinSpawnDist, rand() % (GC::EnemySpawn)+GC::MinSpawnDist + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Down) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawn)+GC::MinSpawnDist, rand() % (GC::EnemySpawnHalf)+GC::MinSpawnDist + GC::LowerBounds.y));
	}
	else if (playerPosition == GC::Left) {
		spr.setPosition(sf::Vector2f(rand() % (GC::EnemySpawnHalf)+(GC::GameSize / 2) + (GC::CharRadius), rand() % (GC::EnemySpawn)+GC::MinSpawnDist + GC::LowerBounds.y));
	}
	active = true;
}

void Mummy::render(sf::RenderWindow& window) {
	if (active) window.draw(spr);
}