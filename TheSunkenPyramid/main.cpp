#include <assert.h>

#include "SFML/Graphics.hpp"
#include "layout.h"
#include "constants.h"
 
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

struct Object {
	enum class ObjT { Player, PlayerProjectile, Enemy, EnemyProjectile };
	ObjT type;
	Sprite spr;
	bool active = false;
	Vector2f velocity = Vector2f(0.f, 0.f);

	void PlayerControl(const Vector2u& screenSize, float elapsedSec) {
		Vector2f pos = spr.getPosition();
		const float SPEED = 180.f;
		velocity = Vector2f(0.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::W)) velocity.y -= SPEED;
		if (Keyboard::isKeyPressed(Keyboard::S)) velocity.y += SPEED;
		if (Keyboard::isKeyPressed(Keyboard::A)) velocity.x -= SPEED;
		if (Keyboard::isKeyPressed(Keyboard::D)) velocity.x += SPEED;
		pos += velocity * elapsedSec;
		spr.setPosition(pos);
		/*if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			
		}*/
	}

	void Update(const Vector2u& screenSize, float elapsed) {
		if (active == true) {
			spr.setColor(Color(255, 255, 255, 255));
			switch (type)
			{
			case ObjT::Player:
				PlayerControl(screenSize, elapsed);
				break;
			}
		}
		else spr.setColor(Color(0, 0, 0, 0));
	}

	void Render(RenderWindow& window) {
		window.draw(spr);
	};

	void InitPlayer(RenderWindow& window, Texture& tex) {
		spr.setTexture(tex);
		IntRect texR = spr.getTextureRect();
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		spr.setPosition(GC::ScreenCentre.x,GC::ScreenCentre.y);
		active = true;
	};

	void Init(RenderWindow& window, Texture& tex)
	{
		switch (type)
		{
		case ObjT::Player:
			InitPlayer(window, tex);
			break;
		default:
			assert(false);
		}
	}
};



int main()
{

	srand(time(NULL));

	int floorNumber = 1;

	RenderWindow window(VideoMode(GC::WindowSize.x, GC::WindowSize.y), "The Sunken Pyramid");

	std::vector<Object>objects;

	Texture playerTex;
	LoadTexture("data/player.png", playerTex);
	Object player;
	player.type = Object::ObjT::Player;
	player.Init(window, playerTex);
	objects.push_back(player);

	Texture stairsTex;
	LoadTexture("data/stairs.png", stairsTex);
	Sprite stairs;
	stairs.setTexture(stairsTex);
	stairs.setPosition(GC::ScreenCentre.x - 16, GC::ScreenCentre.y - 16);

	RectangleShape infoRectangle(Vector2f(512.f, 128.f));
	infoRectangle.setFillColor(Color(128, 128, 128));

	RectangleShape wall(Vector2f(512.f, 8.f));
	wall.setFillColor(Color(255, 0, 0));
	wall.setOrigin(256.f, 256.f);
	wall.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
	wall.setRotation(90);
	
	Clock clock;

	std::vector<std::vector<int>>layout(LG::layoutGeneration());
	std::vector<int>roomPointer{GC::FloorCentre.x, GC::FloorCentre.y};
	std::vector<bool>walls{false, false, false, false};
	wallsCheck(layout, roomPointer, walls);

	//Start game loop
	while (window.isOpen())
	{
		// Process events
		Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == Event::Closed) window.close();

			if (event.type == Event::TextEntered)
			{
				if (event.text.unicode == GC::EscapeKey) {
					window.close();
				}
			}
		}

		window.clear();

		float elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();

		for (size_t i = 0; i < objects.size(); ++i) 
		{
			objects[i].Update(window.getSize(), elapsed);	
		}

		//Is playing moving into a wall?
		if (objects[0].spr.getPosition().y < GC::LowerBounds.y + 24 && walls[0] == true) objects[0].spr.setPosition(objects[0].spr.getPosition().x, GC::LowerBounds.y + 24);
		if (objects[0].spr.getPosition().x > GC::WindowSize.x - 24 && walls[1] == true) objects[0].spr.setPosition(GC::WindowSize.x - 24, objects[0].spr.getPosition().y);
		if (objects[0].spr.getPosition().y > GC::WindowSize.y - 24 && walls[2] == true) objects[0].spr.setPosition(objects[0].spr.getPosition().x, GC::WindowSize.y - 24);
		if (objects[0].spr.getPosition().x < GC::LowerBounds.x + 24 && walls[3] == true) objects[0].spr.setPosition(GC::LowerBounds.x + 24, objects[0].spr.getPosition().y);
		
		if (objects[0].spr.getPosition().y < GC::LowerBounds.y + 8 && walls[0] == false) {
			objects[0].spr.setPosition(objects[0].spr.getPosition().x, GC::WindowSize.y - 24);
			roomPointer[0] -= 1;
			wallsCheck(layout, roomPointer, walls);
		}
		if (objects[0].spr.getPosition().x > GC::WindowSize.x - 8 && walls[1] == false) {
			objects[0].spr.setPosition(GC::LowerBounds.x + 24, objects[0].spr.getPosition().y);
			roomPointer[1] += 1;
			wallsCheck(layout, roomPointer, walls);
		}
		if (objects[0].spr.getPosition().y > GC::WindowSize.y - 8 && walls[2] == false) {
			objects[0].spr.setPosition(objects[0].spr.getPosition().x, GC::LowerBounds.y + 24);
			roomPointer[0] += 1;
			wallsCheck(layout, roomPointer, walls);
		}
		if (objects[0].spr.getPosition().x < GC::LowerBounds.x + 8 && walls[3] == false) {
			objects[0].spr.setPosition(GC::WindowSize.x - 24, objects[0].spr.getPosition().y);
			roomPointer[1] -= 1;
			wallsCheck(layout, roomPointer, walls);
		}

		if (layout[roomPointer[0]][roomPointer[1]] == 4)
		{
			window.draw(stairs);
			if (objects[0].spr.getPosition().x < GC::ScreenCentre.x + 32 && objects[0].spr.getPosition().x > GC::ScreenCentre.x - 32 && objects[0].spr.getPosition().y > GC::ScreenCentre.y - 32 && objects[0].spr.getPosition().y < GC::ScreenCentre.y + 32)
			{
				std::cout << "Floor clear\n";
				objects[0].spr.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
				roomPointer = { GC::FloorCentre.x, GC::FloorCentre.y };
				++floorNumber;
				layout = LG::layoutGeneration(floorNumber);
				wallsCheck(layout, roomPointer, walls);
			}
		}

		if (walls[0]) {
			wall.setRotation(0);
			window.draw(wall);
		}
		if (walls[1]) {
			wall.setRotation(90);
			window.draw(wall);
		}
		if (walls[2]) {
			wall.setRotation(180);
			window.draw(wall);
		}
		if (walls[3]) {
			wall.setRotation(270);
			window.draw(wall);
		}

		for (size_t i = 0; i < objects.size(); ++i) {
			objects[i].Render(window);
		}

		window.draw(infoRectangle);

		window.display();
	}
};