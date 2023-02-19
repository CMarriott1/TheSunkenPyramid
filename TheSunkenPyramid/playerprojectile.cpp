#include "playerprojectile.h"
#include "SFML/Graphics.hpp"
#include "constants.h"

void PlayerProjectile::init(sf::Texture& tex) {
	active = false;
	spr.setTexture(tex);
	sf::IntRect texR = spr.getTextureRect();
	spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
};

void PlayerProjectile::update(float elapsedSec) {
	if (active == true)
	{
		sf::Vector2f pos = spr.getPosition();
		pos += velocity * elapsedSec;
		spr.setPosition(pos);
		if (spr.getPosition().x < 0 || spr.getPosition().x > 512 || spr.getPosition().y < 128 || spr.getPosition().y > 640)
		{
			active = false;
		}
	}
}

void PlayerProjectile::activate(int direction, const sf::Vector2f& playerPosition, std::vector<int>offset) {
	direction = (direction + offset[1]) % 4;
	if (direction == GC::Up) {
		velocity = sf::Vector2f(0.f, -GC::PlayerProjectileSpeed);
		spr.setPosition(playerPosition.x + offset[0], playerPosition.y - GC::CharRadius);
	}
	else if (direction == GC::Right) {
		velocity = sf::Vector2f(GC::PlayerProjectileSpeed, 0.f);
		spr.setPosition(playerPosition.x + GC::CharRadius, playerPosition.y + offset[0]);
	}
	else if (direction == GC::Down) {
		velocity = sf::Vector2f(0.f, GC::PlayerProjectileSpeed);
		spr.setPosition(playerPosition.x - offset[0], playerPosition.y + GC::CharRadius);
	}
	else if (direction == GC::Left) {
		velocity = sf::Vector2f(-GC::PlayerProjectileSpeed, 0.f);
		spr.setPosition(playerPosition.x - GC::CharRadius, playerPosition.y - offset[0]);
	}
	active = true;
}

void PlayerProjectile::render(sf::RenderWindow& window)
{
	if (active) window.draw(spr);
}