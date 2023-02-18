#include "enemyprojectile.h"
#include "SFML/Graphics.hpp"
#include "constants.h"
#include "math.h"

void EnemyProjectile::init(sf::Texture& tex) {
	active = false;
	spr.setTexture(tex);
	sf::IntRect texR = spr.getTextureRect();
	spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
};

void EnemyProjectile::update(float elapsedSec) {
	if (active == true)
	{
		sf::Vector2f pos = spr.getPosition();
		pos += velocity * elapsedSec *  GC::EnemyProjectileSpeed;
		spr.setPosition(pos);
		if (spr.getPosition().x < 0 || spr.getPosition().x > 512 || spr.getPosition().y < 128 || spr.getPosition().y > 640)
		{
			active = false;
		}
	}
}

void EnemyProjectile::activate(const sf::Vector2f& enemyPosition, const sf::Vector2f& playerPosition) {
	spr.setPosition(enemyPosition);
	const float xdiff = playerPosition.x - enemyPosition.x;
	const float ydiff = playerPosition.y - enemyPosition.y;
	const float hypot = sqrt((xdiff * xdiff) + (ydiff * ydiff));
	velocity.x = xdiff / hypot;
	velocity.y = ydiff / hypot;
	active = true;
}

void EnemyProjectile::render(sf::RenderWindow& window)
{
	if (active) window.draw(spr);
}