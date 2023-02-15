#include "SFML/Graphics.hpp"
#include "layout.h"
#include "constants.h"
#include <assert.h>
#include "player.h"


 
using namespace sf;

bool LoadTexture(const std::string& file, Texture& tex)
{
	if (tex.loadFromFile(file))
	{
		tex.setSmooth(true);
		return true;
	}
	assert(false);
	return false;
}

void wallsCheck(std::vector<std::vector<int>>layout, std::vector<int>roomPointer, std::vector<bool>&walls)
{	
	walls = { false,false,false,false };
	if (layout[roomPointer[0] - 1][roomPointer[1]] == 0) walls[0] = true;
	if (layout[roomPointer[0]][roomPointer[1] + 1] == 0) walls[1] = true;
	if (layout[roomPointer[0] + 1][roomPointer[1]] == 0) walls[2] = true;
	if (layout[roomPointer[0]][roomPointer[1] - 1] == 0) walls[3] = true;
}

int main()
{
	//Initialisation

	srand(time(NULL));

	int floorNumber = 1;

	RenderWindow window(VideoMode(GC::WindowSize.x, GC::WindowSize.y), "The Sunken Pyramid");

	window.setKeyRepeatEnabled(false);
	
	Texture playerTex;
	LoadTexture("data/player.png", playerTex);
	Player player;
	player.init(playerTex);

	/*Texture playerProjectileTex;
	LoadTexture("data/playerProjectile.png", playerProjectileTex);
	Object playerProjectile;
	playerProjectile.type = Object::ObjT::PlayerProjectile;
	objects.insert(objects.end(), 3, playerProjectile);*/
	

	Texture stairsTex;
	LoadTexture("data/stairs.png", stairsTex);
	Sprite stairs;
	stairs.setTexture(stairsTex);
	stairs.setPosition(GC::ScreenCentre.x - 16, GC::ScreenCentre.y - 16);

	RectangleShape infoRectangle(Vector2f(512.f, 128.f));
	infoRectangle.setFillColor(Color(128, 128, 128));

	RectangleShape wallbase(Vector2f(496.f, 496.f));
	wallbase.setOutlineColor(Color(255, 0, 0));
	wallbase.setOutlineThickness(8);
	wallbase.setFillColor(Color::Transparent);
	wallbase.setOrigin(248.f, 248.f);
	wallbase.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
	
	RectangleShape wall(Vector2f(128.f, 8.f));
	wall.setFillColor(Color(0, 0, 0));
	wall.setOrigin(64.f, 256.f);
	wall.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
	wall.setRotation(90);

	Clock clock;

	std::vector<std::vector<int>>layout(LG::layoutGeneration());
	std::vector<int>roomPointer{GC::FloorCentre.x, GC::FloorCentre.y};
	std::vector<bool>walls{false, false, false, false};
	wallsCheck(layout, roomPointer, walls);

	Event event;

	//Start game loop
	while (window.isOpen())
	{
		// Process events
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == Event::Closed) window.close();

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape) {
					window.close();
				}
			}
		}

		window.clear();

		float elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();

		/*for (size_t i = 0; i < objects.size(); ++i) 
		{
			objects[i].Update(window.getSize(), elapsed);	
		}*/

		player.update(window.getSize(), elapsed);

		//Is playing moving into a wall?
		if (player.spr.getPosition().y < GC::LowerBounds.y + 24 && walls[0] == true) player.spr.setPosition(player.spr.getPosition().x, GC::LowerBounds.y + 24);
		if (player.spr.getPosition().x > GC::WindowSize.x - 24 && walls[1] == true) player.spr.setPosition(GC::WindowSize.x - 24, player.spr.getPosition().y);
		if (player.spr.getPosition().y > GC::WindowSize.y - 24 && walls[2] == true) player.spr.setPosition(player.spr.getPosition().x, GC::WindowSize.y - 24);
		if (player.spr.getPosition().x < GC::LowerBounds.x + 24 && walls[3] == true) player.spr.setPosition(GC::LowerBounds.x + 24, player.spr.getPosition().y);
		
		//Is player moving into an exit?
		if (player.spr.getPosition().y < GC::LowerBounds.y + 8 && walls[0] == false) {
			player.spr.setPosition(player.spr.getPosition().x, GC::WindowSize.y - 24);
			roomPointer[0] -= 1;
			wallsCheck(layout, roomPointer, walls);
		}
		if (player.spr.getPosition().x > GC::WindowSize.x - 8 && walls[1] == false) {
			player.spr.setPosition(GC::LowerBounds.x + 24, player.spr.getPosition().y);
			roomPointer[1] += 1;
			wallsCheck(layout, roomPointer, walls);
		}
		if (player.spr.getPosition().y > GC::WindowSize.y - 8 && walls[2] == false) {
			player.spr.setPosition(player.spr.getPosition().x, GC::LowerBounds.y + 24);
			roomPointer[0] += 1;
			wallsCheck(layout, roomPointer, walls);
		}
		if (player.spr.getPosition().x < GC::LowerBounds.x + 8 && walls[3] == false) {
			player.spr.setPosition(GC::WindowSize.x - 24, player.spr.getPosition().y);
			roomPointer[1] -= 1;
			wallsCheck(layout, roomPointer, walls);
		}

		//Room types
		if (layout[roomPointer[0]][roomPointer[1]] == 4)
		{
			window.draw(stairs);
			if (player.spr.getPosition().x < GC::ScreenCentre.x + 32 && player.spr.getPosition().x > GC::ScreenCentre.x - 32 && player.spr.getPosition().y > GC::ScreenCentre.y - 32 && player.spr.getPosition().y < GC::ScreenCentre.y + 32)
			{
				std::cout << "Floor clear\n";
				player.spr.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
				roomPointer = { GC::FloorCentre.x, GC::FloorCentre.y };
				++floorNumber;
				layout = LG::layoutGeneration(floorNumber);
				wallsCheck(layout, roomPointer, walls);
			}
		}

		//Rendering

		window.draw(wallbase);
		if (!walls[0]) {
			wall.setRotation(0);
			window.draw(wall);
		}
		if (!walls[1]) {
			wall.setRotation(90);
			window.draw(wall);
		}
		if (!walls[2]) {
			wall.setRotation(180);
			window.draw(wall);
		}
		if (!walls[3]) {
			wall.setRotation(270);
			window.draw(wall);
		}

		/*for (size_t i = 0; i < objects.size(); ++i) {
			objects[i].Render(window);
		}*/
		window.draw(player.spr);
		window.draw(infoRectangle);

		//The thing that matters
		window.display();
	}
};