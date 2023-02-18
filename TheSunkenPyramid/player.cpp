#include "player.h"
#include "SFML/Graphics.hpp"
#include "constants.h"

void Player::init(sf::Texture& tex) 
{
	maxHealth = 3;
	health = maxHealth;
	invulnerability = 0;
	velocity = sf::Vector2f(0.f, 0.f);
	spr.setTexture(tex);
	sf::IntRect texR = spr.getTextureRect();
	spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
	spr.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
	active = true;
};

void Player::update(const sf::Vector2u& screenSize, float elapsedSec) 
{
	sf::Vector2f pos = spr.getPosition();
	velocity = sf::Vector2f(0.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) velocity.y -= GC::PlayerSpeed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) velocity.y += GC::PlayerSpeed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x -= GC::PlayerSpeed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x += GC::PlayerSpeed;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) fireDirection = GC::Up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) fireDirection = GC::Right;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) fireDirection = GC::Down;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) fireDirection = GC::Left;
	else fireDirection = -1;

	pos += velocity * elapsedSec;
	spr.setPosition(pos);

	invulnerability -= elapsedSec;
}

void Player::hurt()
{
	if (invulnerability <= 0)
	{
		--health;
		invulnerability = 1;

		if (health <= 0)
		{

		}
	}
}
