#include <assert.h>

#include "SFML/Graphics.hpp"
#include "layout.h"
#include "constants.h"
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

class Object {
public:
	enum class ObjT { Player, PlayerProjectile, Enemy, EnemyProjectile };
	ObjT type;
	Sprite spr;
	bool active = false;
	Vector2f velocity = Vector2f(0.f, 0.f);
	float cooldown = 0.f;

	void PlayerControl(const Vector2u& screenSize, float elapsedSec) {
		Vector2f pos = spr.getPosition();
		const float SPEED = 180.f;
		velocity = Vector2f(0.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::W)) velocity.y -= SPEED;
		if (Keyboard::isKeyPressed(Keyboard::S)) velocity.y += SPEED;
		if (Keyboard::isKeyPressed(Keyboard::A)) velocity.x -= SPEED;
		if (Keyboard::isKeyPressed(Keyboard::D)) velocity.x += SPEED;

		/*if (cooldown <= 0)
		{
			if(Keyboard::isKeyPressed(Keyboard::)
		}*/
		pos += velocity * elapsedSec;
		spr.setPosition(pos);
	}

	void UpdatePlayerProjectile() {
		spr.setPosition(Vector2f(spr.getPosition().x + velocity.x, spr.getPosition().y + velocity.y));
	}

	void Update(const Vector2u& screenSize, float elapsed) {
		if (active == true) {
			spr.setColor(Color(255, 255, 255, 255));
			switch (type)
			{
			case ObjT::Player:
				PlayerControl(screenSize, elapsed);
				break;
			case ObjT::PlayerProjectile:
				UpdatePlayerProjectile();
				break;
			}
		}
		else spr.setColor(Color(0, 0, 0, 0));
	}

	void Render(RenderWindow& window) {
		window.draw(spr);
	};

	void InitPlayer(Texture& tex) {
		spr.setTexture(tex);
		IntRect texR = spr.getTextureRect();
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		spr.setPosition(GC::ScreenCentre.x,GC::ScreenCentre.y);
		active = true;
	};

	void InitPlayerProjectile(RenderWindow& window, Texture& tex) {
		spr.setTexture(tex);
		IntRect texR = spr.getTextureRect();
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
	}

	void Init(RenderWindow& window, Texture& tex)
	{
		switch (type)
		{
		case ObjT::Player:
			InitPlayer(tex);
			break;
		case ObjT::PlayerProjectile:
			InitPlayerProjectile(window, tex);
			break;
		default:
			assert(false);
		}
	}

	void activatePlayerProjectile(Vector2i position, Vector2i velocity) {

	}
};



int main()
{
	//Initialisation

	srand(time(NULL));

	int floorNumber = 1;

	RenderWindow window(VideoMode(GC::WindowSize.x, GC::WindowSize.y), "The Sunken Pyramid");

	window.setKeyRepeatEnabled(false);

	std::vector<Object>objects;
	
	Texture playerTex;
	LoadTexture("data/player.png", playerTex);
	Object player;
	player.type = Object::ObjT::Player;
	player.Init(window, playerTex);
	objects.push_back(player);

	Texture playerProjectileTex;
	LoadTexture("data/playerProjectile.png", playerProjectileTex);
	Object playerProjectile;
	playerProjectile.type = Object::ObjT::PlayerProjectile;
	objects.insert(objects.end(), 3, playerProjectile);
	

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

		for (size_t i = 0; i < objects.size(); ++i) 
		{
			objects[i].Update(window.getSize(), elapsed);	
		}

		//Is playing moving into a wall?
		if (objects[0].spr.getPosition().y < GC::LowerBounds.y + 24 && walls[0] == true) objects[0].spr.setPosition(objects[0].spr.getPosition().x, GC::LowerBounds.y + 24);
		if (objects[0].spr.getPosition().x > GC::WindowSize.x - 24 && walls[1] == true) objects[0].spr.setPosition(GC::WindowSize.x - 24, objects[0].spr.getPosition().y);
		if (objects[0].spr.getPosition().y > GC::WindowSize.y - 24 && walls[2] == true) objects[0].spr.setPosition(objects[0].spr.getPosition().x, GC::WindowSize.y - 24);
		if (objects[0].spr.getPosition().x < GC::LowerBounds.x + 24 && walls[3] == true) objects[0].spr.setPosition(GC::LowerBounds.x + 24, objects[0].spr.getPosition().y);
		
		//Is player moving into an exit?
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

		//Room types
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

		for (size_t i = 0; i < objects.size(); ++i) {
			objects[i].Render(window);
		}

		window.draw(infoRectangle);

		//The thing that matters
		window.display();
	}
};