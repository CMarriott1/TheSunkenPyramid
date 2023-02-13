#include "SFML/Graphics.hpp"
#include "constants.h"

struct Player {
	sf::Sprite spr;
	bool active = false;
	sf::Vector2f velocity = sf::Vector2f(0.f, 0.f);
	float cooldown = 0.f;

	void update(const sf::Vector2u& screenSize, float elapsedSec) {
		sf::Vector2f pos = spr.getPosition();
		const float SPEED = 180.f;
		velocity = sf::Vector2f(0.f, 0.f);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) velocity.y -= SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) velocity.y += SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x -= SPEED;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x += SPEED;

		/*if (cooldown <= 0)
		{
			if(Keyboard::isKeyPressed(Keyboard::)
		}*/
		pos += velocity * elapsedSec;
		spr.setPosition(pos);
	}

	void init(sf::Texture& tex) {
		spr.setTexture(tex);
		sf::IntRect texR = spr.getTextureRect();
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		spr.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
		active = true;
	};
};