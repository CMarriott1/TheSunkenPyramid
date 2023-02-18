#include "SFML/Graphics.hpp"
#include "layout.h"
#include "constants.h"
#include <assert.h>

#include "player.h"
#include "playerprojectile.h"
#include "bat.h"
#include "mummy.h"
#include "enemyprojectile.h"

 
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

void wallsCheck(std::vector<std::vector<int>>&layout, std::vector<int>&roomPointer, std::vector<bool>&walls)
{	
	walls = { false,false,false,false };
	if (layout[roomPointer[0] - 1][roomPointer[1]] == 0) walls[0] = true;
	if (layout[roomPointer[0]][roomPointer[1] + 1] == 0) walls[1] = true;
	if (layout[roomPointer[0] + 1][roomPointer[1]] == 0) walls[2] = true;
	if (layout[roomPointer[0]][roomPointer[1] - 1] == 0) walls[3] = true;
}

void newRoom(std::vector<std::vector<int>>& layout, std::vector<int>& roomPointer, std::vector<bool>& walls, std::vector<PlayerProjectile>& projectiles, int playerPosition, int floorNumber, std::vector<Bat>& bats, std::vector<Mummy>& mummies, int& roomType, int& enemyCounter)
{
	wallsCheck(layout, roomPointer, walls);
	for (size_t i = 0; i < projectiles.size(); ++i)
	{
		projectiles[i].active = false;
	}

	roomType = layout[roomPointer[0]][roomPointer[1]];
	if (roomType == 2)
	{
		for (size_t i = 0; i < floorNumber; ++i)
		{
			if (rand() % 2 == 1) bats[i].activate(playerPosition);
			else mummies[i].activate(playerPosition);
		}
		walls = { true, true, true, true };
		enemyCounter = floorNumber;
	}
}

bool collision(const Vector2f& pos1, const Vector2f& pos2, int radii)
{
	const float xdiff = pos1.x - pos2.x;
	const float ydiff = pos1.y - pos2.y;
	return ((xdiff * xdiff) + (ydiff * ydiff) <= (radii * radii));
}

int main()
{
	//Initialisation

	srand(time(NULL));

	int floorNumber = 1;
	int roomType = 1;
	int enemyCounter = 0;

	RenderWindow window(VideoMode(GC::WindowSize.x, GC::WindowSize.y), "The Sunken Pyramid");

	window.setKeyRepeatEnabled(false);
	
	Texture playerTex;
	LoadTexture("data/player.png", playerTex);
	Player player;
	player.init(playerTex);

	Texture playerProjectileTex;
	LoadTexture("data/playerProjectile.png", playerProjectileTex);
	PlayerProjectile playerProjectile;
	playerProjectile.init(playerProjectileTex);
	std::vector<PlayerProjectile>playerProjectiles;
	playerProjectiles.insert(playerProjectiles.begin(), 3, playerProjectile);

	Texture batTex;
	LoadTexture("data/bat.png", batTex);
	Bat bat;
	bat.init(batTex);
	std::vector<Bat>bats;
	bats.insert(bats.begin(), 4, bat);

	Texture mummyTex;
	LoadTexture("data/mummy.png", mummyTex);
	Mummy mummy;
	mummy.init(mummyTex);
	std::vector<Mummy>mummies;
	mummies.insert(mummies.begin(), 4, mummy);

	Texture enemyProjectileTex;
	LoadTexture("data/enemyProjectile.png", enemyProjectileTex);
	EnemyProjectile enemyProjectile;
	enemyProjectile.init(enemyProjectileTex);
	std::vector<EnemyProjectile>enemyProjectiles;
	enemyProjectiles.insert(enemyProjectiles.begin(), 3, enemyProjectile);

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

	float firetimer = 0;

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

		//Player processing
		for (size_t i = 0; i < playerProjectiles.size(); ++i)
		{
			if (playerProjectiles[i].active) playerProjectiles[i].update(elapsed);
		}

		firetimer -= elapsed;
		player.update(window.getSize(), elapsed);
		if (firetimer <= 0)
		{
			if (player.fireDirection != -1)
			{
				for (int i = 0; i < playerProjectiles.size(); ++i)
				{
					if (playerProjectiles[i].active == false)
					{
						playerProjectiles[i].activate(player.fireDirection, player.spr.getPosition());
						firetimer = GC::PlayerFiringSpeed;
						break;
					}
				}
			}
		}

		//Is playing moving into a wall?
		if (player.spr.getPosition().y < GC::LowerBounds.y + 24 && walls[0] == true) player.spr.setPosition(player.spr.getPosition().x, GC::LowerBounds.y + 24);
		if (player.spr.getPosition().x > GC::WindowSize.x - 24 && walls[1] == true) player.spr.setPosition(GC::WindowSize.x - 24, player.spr.getPosition().y);
		if (player.spr.getPosition().y > GC::WindowSize.y - 24 && walls[2] == true) player.spr.setPosition(player.spr.getPosition().x, GC::WindowSize.y - 24);
		if (player.spr.getPosition().x < GC::LowerBounds.x + 24 && walls[3] == true) player.spr.setPosition(GC::LowerBounds.x + 24, player.spr.getPosition().y);

		//Is player moving into an exit?
		if (player.spr.getPosition().y < GC::LowerBounds.y + 8 && walls[0] == false) {
			player.spr.setPosition(player.spr.getPosition().x, GC::WindowSize.y - 24);
			roomPointer[0] -= 1;
			newRoom(layout, roomPointer, walls, playerProjectiles, GC::Down, floorNumber, bats, mummies, roomType, enemyCounter);
		}
		if (player.spr.getPosition().x > GC::WindowSize.x - 8 && walls[1] == false) {
			player.spr.setPosition(GC::LowerBounds.x + 24, player.spr.getPosition().y);
			roomPointer[1] += 1;
			newRoom(layout, roomPointer, walls, playerProjectiles, GC::Left, floorNumber, bats, mummies, roomType, enemyCounter);
		}
		if (player.spr.getPosition().y > GC::WindowSize.y - 8 && walls[2] == false) {
			player.spr.setPosition(player.spr.getPosition().x, GC::LowerBounds.y + 24);
			roomPointer[0] += 1;
			newRoom(layout, roomPointer, walls, playerProjectiles, GC::Up, floorNumber, bats, mummies, roomType, enemyCounter);
		}
		if (player.spr.getPosition().x < GC::LowerBounds.x + 8 && walls[3] == false) {
			player.spr.setPosition(GC::WindowSize.x - 24, player.spr.getPosition().y);
			roomPointer[1] -= 1;
			newRoom(layout, roomPointer, walls, playerProjectiles, GC::Right, floorNumber, bats, mummies, roomType, enemyCounter);
		}

		//Player related collision processing
		if (roomType == 2)
		{
			for (size_t projectile = 0; projectile < playerProjectiles.size(); ++projectile)
			{
				if (playerProjectiles[projectile].active)
				{
					for (size_t enemy = 0; enemy < bats.size() && playerProjectiles[projectile].active; ++enemy)
					{
						if (bats[enemy].active)
						{
							if (collision(playerProjectiles[projectile].spr.getPosition(), bats[enemy].spr.getPosition(), GC::ProjRadius + GC::CharRadius))
							{
								enemyCounter -= bats[enemy].hurt();
								playerProjectiles[projectile].active = false;
							}
						}
						else if (mummies[enemy].active)
						{
							if (collision(playerProjectiles[projectile].spr.getPosition(), mummies[enemy].spr.getPosition(), GC::ProjRadius + GC::CharRadius))
							{
								enemyCounter -= mummies[enemy].hurt();
								playerProjectiles[projectile].active = false;
							}
						}
					}
				}
			}
			if (enemyCounter == 0) {
				layout[roomPointer[0]][roomPointer[1]] = 1;
				roomType = 1;
				wallsCheck(layout, roomPointer, walls);
			}
		}


		//Enemy processing
		for (size_t i = 0; i < bats.size(); ++i)
		{
			if (bats[i].active) bats[i].update(player.spr.getPosition(), elapsed);
		}
		for (size_t i = 0; i < mummies.size(); ++i)
		{
			if (mummies[i].active)
			{
				if (mummies[i].update(elapsed))
				{
					for (size_t j = 0; j < enemyProjectiles.size(); ++j)
					{
						if (!enemyProjectiles[j].active)
						{
							enemyProjectiles[j].activate(mummies[i].spr.getPosition(), player.spr.getPosition());
							break;
						}
					}
				}
			}
		}
		for (size_t i = 0; i < enemyProjectiles.size(); ++i)
		{
			if (enemyProjectiles[i].active) enemyProjectiles[i].update(elapsed);
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
		window.draw(player.spr);

		for (size_t i = 0; i < playerProjectiles.size(); ++i) {
			playerProjectiles[i].render(window);
		}
		for (size_t i = 0; i < bats.size(); ++i) {
			bats[i].render(window);
		}
		for (size_t i = 0; i < mummies.size(); ++i) {
			mummies[i].render(window);
		}
		for (size_t i = 0; i < enemyProjectiles.size(); ++i) {
			enemyProjectiles[i].render(window);
		}
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

		window.draw(infoRectangle);


		//The thing that matters
		window.display();
	}
};