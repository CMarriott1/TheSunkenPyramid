#include "jewel.h"
#include "SFML/Graphics.hpp"
#include "constants.h"

void Jewel::init(sf::Texture& tex, int Type) {
	jewelType = Type;
	found = false;
	tex.setSmooth(false);
	spr.setTexture(tex);
	spr.setScale(4.f, 4.f);
	sf::IntRect texR = spr.getTextureRect();
	spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
	spr.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
};

void Jewel::collect(int index) 
{
	spr.setScale(8.f, 8.f);
	spr.setPosition(388.f + (index%2 * 72), 32 +  (index/2 * 64));
	found = true;
}