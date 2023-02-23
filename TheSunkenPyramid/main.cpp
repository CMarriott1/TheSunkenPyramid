#include "SFML/Graphics.hpp"
#include "layout.h"
#include "constants.h"
#include <assert.h>

#include "player.h"
#include "playerprojectile.h"
#include "bat.h"
#include "mummy.h"
#include "enemyprojectile.h"
#include "jewel.h"

 
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

void newRoom(std::vector<std::vector<int>>& layout, std::vector<int>& roomPointer, std::vector<bool>& walls, std::vector<PlayerProjectile>& projectiles, std::vector<EnemyProjectile>& enemyProjectiles , int playerPosition, int floorNumber, std::vector<Bat>& bats, std::vector<Mummy>& mummies, int& roomType, int& enemyCounter)
{
	//Wall update
	wallsCheck(layout, roomPointer, walls);

	//Projectile Clearing
	for (size_t i = 0; i < projectiles.size(); ++i)
	{
		projectiles[i].active = false;
	}
	for (size_t i = 0; i < enemyProjectiles.size(); ++i)
	{
		enemyProjectiles[i].active = false;
	}
	
	//Room type update 
	roomType = layout[roomPointer[0]][roomPointer[1]];

	//Enemy room initialisation
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

void fire(std::vector<bool>& items, std::vector<PlayerProjectile>& projectiles, Player& player)
{
	//Initialisation
	int projectileCount = 1;
	if (items[0]) projectileCount += 1;
	if (items[2]) projectileCount += 1;
	if (items[3]) projectileCount += 2;

	std::vector<int> inactiveProjectiles;

	for (int i = 0; i < projectiles.size() && inactiveProjectiles.size()  < projectileCount; ++i)
	{
		if (projectiles[i].active == false)
		{
			inactiveProjectiles.push_back(i);
		}
	}
	int projectileIndex = 0;

	//Projectile Creation
	if (items[0])
	{
		projectiles[inactiveProjectiles[projectileIndex]].activate(player.fireDirection, player.spr.getPosition(),GC::DoubleShotLeft,items[1]);
		++projectileIndex;
		projectiles[inactiveProjectiles[projectileIndex]].activate(player.fireDirection, player.spr.getPosition(), GC::DoubleShotRight, items[1]);
		++projectileIndex;
	}
	else
	{
		projectiles[inactiveProjectiles[projectileIndex]].activate(player.fireDirection, player.spr.getPosition(),GC::StandardShot,items[1]);
		++projectileIndex;
	}
	if (items[2])
	{
		projectiles[inactiveProjectiles[projectileIndex]].activate(player.fireDirection, player.spr.getPosition(),GC::BackShot, items[1]);
		++projectileIndex;
	}
	if (items[3])
	{
		projectiles[inactiveProjectiles[projectileIndex]].activate(player.fireDirection, player.spr.getPosition(),GC::RightShot, items[1]);
		++projectileIndex;
		projectiles[inactiveProjectiles[projectileIndex]].activate(player.fireDirection, player.spr.getPosition(),GC::LeftShot, items[1]);
		++projectileIndex;
	}
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
	
	//Menu Sprites
	Texture mainMenuTex;
	LoadTexture("data/mainmenu.png", mainMenuTex);
	Sprite mainMenu;
	mainMenu.setTexture(mainMenuTex);

	Texture menuSelectTex;
	LoadTexture("data/menuselect.png", menuSelectTex);
	Sprite menuSelect;
	menuSelect.setTexture(menuSelectTex);
	menuSelect.setPosition(154, 401);

	//Game Sprites
	Texture playerTex;
	LoadTexture("data/player.png", playerTex);
	Player player;
	player.init(playerTex);

	Texture playerProjectileTex;
	LoadTexture("data/playerProjectile.png", playerProjectileTex);
	PlayerProjectile playerProjectile;
	playerProjectile.init(playerProjectileTex);
	std::vector<PlayerProjectile>playerProjectiles;
	playerProjectiles.insert(playerProjectiles.begin(), 30, playerProjectile);

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
	enemyProjectiles.insert(enemyProjectiles.begin(), 12, enemyProjectile);

	Texture stairsTex;
	LoadTexture("data/stairs.png", stairsTex);
	Sprite stairs;
	stairs.setTexture(stairsTex);
	stairs.setPosition(GC::ScreenCentre.x - 16, GC::ScreenCentre.y - 16);

	Texture heartContainerTex;
	LoadTexture("data/heartContainer.png", heartContainerTex);
	heartContainerTex.setSmooth(false);
	Sprite heartContainer;
	heartContainer.setTexture(heartContainerTex);
	heartContainer.setScale(8, 8);

	Texture heartTex;
	LoadTexture("data/heart.png", heartTex);
	heartTex.setSmooth(false);
	Sprite heart;
	heart.setTexture(heartTex);
	heart.setScale(8, 8);

	Texture whiteJewelTex;
	LoadTexture("data/whiteJewel.png", whiteJewelTex);
	Texture purpleJewelTex;
	LoadTexture("data/purpleJewel.png", purpleJewelTex);
	Texture blueJewelTex;
	LoadTexture("data/blueJewel.png", blueJewelTex);
	Texture yellowJewelTex;
	LoadTexture("data/yellowJewel.png", yellowJewelTex);
	Jewel jewel;
	std::vector<Jewel>jewels;
	jewels.insert(jewels.begin(), 4, jewel);
	jewels[0].init(whiteJewelTex,0);
	jewels[1].init(purpleJewelTex,1);
	jewels[2].init(blueJewelTex,2);
	jewels[3].init(yellowJewelTex,3);
	std::random_shuffle(jewels.begin(), jewels.end());

	RectangleShape infoRectangle(Vector2f(512.f, 128.f));
	infoRectangle.setFillColor(Color(128, 128, 128));

	RectangleShape wallbase(Vector2f(496.f, 496.f));
	Color lightStone = Color(229, 229, 139, 255);
	wallbase.setOutlineColor(lightStone);
	wallbase.setOutlineThickness(8);
	wallbase.setFillColor(Color::Transparent);
	wallbase.setOrigin(248.f, 248.f);
	wallbase.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
	
	RectangleShape wall(Vector2f(128.f, 8.f));
	Color darkStone = Color(160, 126, 32, 255);
	wall.setFillColor(darkStone);
	wall.setOrigin(64.f, 256.f);
	wall.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
	wall.setRotation(90);

	Font font;
	font.loadFromFile("data/bgothl.ttf");
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setString("250");
	scoreText.setPosition(20, 88);
	Text floorText;
	floorText.setFont(font);
	floorText.setCharacterSize(24);
	floorText.setString("Floor: 1");
	floorText.setPosition(112, 88);
	floorText.setFillColor(Color(127, 255, 255));

	//More variable initialisation
	Clock clock;

	std::vector<std::vector<int>>layout(LG::layoutGeneration());
	std::vector<int>roomPointer{GC::FloorCentre.x, GC::FloorCentre.y};
	std::vector<bool>walls{false, false, false, false};
	wallsCheck(layout, roomPointer, walls);

	Event event;

	float secondtimer = 1;
	float firetimer = 0;
	std::vector<bool>items(4, false);
	bool loopbreak = false;
	int score = 250;
	
	//Main Menu
	while (!loopbreak)
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape) window.close();
				if (event.key.code == Keyboard::Enter) loopbreak = true;
				if (event.key.code == Keyboard::Up || event.key.code == Keyboard::W) menuSelect.setPosition(154, 305);
				else if (event.key.code == Keyboard::Down || event.key.code == Keyboard::S) menuSelect.setPosition(154, 401);
			}
			window.clear();
			window.draw(mainMenu);
			window.draw(menuSelect);
			window.display();
		}
	}
	loopbreak = false;
	//Post main menu content
	while (window.isOpen())
	{
		//Gane loop
		while (!loopbreak)
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

			window.clear(darkStone);

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
					fire(items, playerProjectiles, player);
					firetimer = GC::PlayerFiringSpeed;
				}
			}

			//Is playing moving into a wall?
			if (player.spr.getPosition().y < GC::LowerBounds.y + (GC::WallSize + GC::CharRadius) && walls[0] == true) player.spr.setPosition(player.spr.getPosition().x, GC::LowerBounds.y + 24);
			if (player.spr.getPosition().x > GC::WindowSize.x - 24 && walls[1] == true) player.spr.setPosition(GC::WindowSize.x - 24, player.spr.getPosition().y);
			if (player.spr.getPosition().y > GC::WindowSize.y - 24 && walls[2] == true) player.spr.setPosition(player.spr.getPosition().x, GC::WindowSize.y - 24);
			if (player.spr.getPosition().x < GC::LowerBounds.x + 24 && walls[3] == true) player.spr.setPosition(GC::LowerBounds.x + 24, player.spr.getPosition().y);

			//Is player moving into an exit?
			if (player.spr.getPosition().y < GC::LowerBounds.y + (GC::WallSize + GC::CharRadius) && walls[0] == false) {
				if (player.spr.getPosition().x > GC::LowerBounds.x + 208 && player.spr.getPosition().x < GC::WindowSize.x - 208)
				{
					player.spr.setPosition(player.spr.getPosition().x, GC::WindowSize.y - (GC::WallSize + GC::CharRadius));
					roomPointer[0] -= 1;
					newRoom(layout, roomPointer, walls, playerProjectiles, enemyProjectiles, GC::Down, floorNumber, bats, mummies, roomType, enemyCounter);
				}
				else player.spr.setPosition(player.spr.getPosition().x, GC::LowerBounds.y + (GC::WallSize + GC::CharRadius));
			}
			if (player.spr.getPosition().x > GC::WindowSize.x - (GC::WallSize + GC::CharRadius) && walls[1] == false) {
				if (player.spr.getPosition().y > GC::LowerBounds.y + 208 && player.spr.getPosition().y < GC::WindowSize.y - 208)
				{
					player.spr.setPosition(GC::LowerBounds.x + (GC::WallSize + GC::CharRadius), player.spr.getPosition().y);
					roomPointer[1] += 1;
					newRoom(layout, roomPointer, walls, playerProjectiles, enemyProjectiles, GC::Left, floorNumber, bats, mummies, roomType, enemyCounter);
				}
				else player.spr.setPosition(GC::WindowSize.x - (GC::WallSize + GC::CharRadius), player.spr.getPosition().y);
			}
			if (player.spr.getPosition().y > GC::WindowSize.y - (GC::WallSize + GC::CharRadius) && walls[2] == false) {
				if (player.spr.getPosition().x > GC::LowerBounds.x + 208 && player.spr.getPosition().x < GC::WindowSize.x - 208)
				{
					player.spr.setPosition(player.spr.getPosition().x, GC::LowerBounds.y + (GC::WallSize + GC::CharRadius));
					roomPointer[0] += 1;
					newRoom(layout, roomPointer, walls, playerProjectiles, enemyProjectiles, GC::Up, floorNumber, bats, mummies, roomType, enemyCounter);
				}
				else player.spr.setPosition(player.spr.getPosition().x, GC::WindowSize.y - (GC::WallSize + GC::CharRadius));
			}
			if (player.spr.getPosition().x < GC::LowerBounds.x + (GC::WallSize + GC::CharRadius) && walls[3] == false) {
				if (player.spr.getPosition().y > GC::LowerBounds.y + 208 && player.spr.getPosition().y < GC::WindowSize.y - 208)
				{
					player.spr.setPosition(GC::WindowSize.x - (GC::WallSize + GC::CharRadius), player.spr.getPosition().y);
					roomPointer[1] -= 1;
					newRoom(layout, roomPointer, walls, playerProjectiles, enemyProjectiles, GC::Right, floorNumber, bats, mummies, roomType, enemyCounter);
				}
				else player.spr.setPosition(GC::LowerBounds.x + (GC::WallSize + GC::CharRadius), player.spr.getPosition().y);
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
								if (collision(playerProjectiles[projectile].spr.getPosition(), bats[enemy].spr.getPosition(), GC::ProjRadius + GC::MummyRadius))
								{
									enemyCounter -= bats[enemy].hurt();
									playerProjectiles[projectile].active = false;
								}
							}
							else if (mummies[enemy].active)
							{
								if (collision(playerProjectiles[projectile].spr.getPosition(), mummies[enemy].spr.getPosition(), GC::ProjRadius + GC::MummyRadius))
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
					score += 25;
					scoreText.setString(std::to_string(score));
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

			//Enemy-related collision processing

			for (size_t projectile = 0; projectile < enemyProjectiles.size(); ++projectile)
			{
				if (enemyProjectiles[projectile].active)
				{
					if (collision(enemyProjectiles[projectile].spr.getPosition(), player.spr.getPosition(), GC::ProjRadius + GC::CharRadius))
					{
						loopbreak = player.hurt(score);
						enemyProjectiles[projectile].active = false;
						scoreText.setString(std::to_string(score));

					}
				}
			}
			for (size_t enemy = 0; enemy < bats.size(); ++enemy)
			{
				if (bats[enemy].active)
				{
					if (collision(bats[enemy].spr.getPosition(), player.spr.getPosition(), GC::CharRadius + GC::BatRadius))
					{
						loopbreak = player.hurt(score);
						scoreText.setString(std::to_string(score));
					}
				}
			}

			//Score decrease
			secondtimer -= elapsed;
			if (secondtimer < 0)
			{
				score -= 1;
				secondtimer = 1;
				scoreText.setString(std::to_string(score));
			}

			//Other room types
			if (layout[roomPointer[0]][roomPointer[1]] == 3)
			{
				window.draw(jewels[floorNumber - 1].spr);
				if (player.spr.getPosition().x < GC::ScreenCentre.x + 32 && player.spr.getPosition().x > GC::ScreenCentre.x - 32 && player.spr.getPosition().y > GC::ScreenCentre.y - 32 && player.spr.getPosition().y < GC::ScreenCentre.y + 32)
				{
					score += 100;
					items[jewels[floorNumber - 1].jewelType] = true;
					jewels[floorNumber - 1].collect(floorNumber - 1);
					layout[roomPointer[0]][roomPointer[1]] = 1;
					scoreText.setString(std::to_string(score));
				}
			}
			if (layout[roomPointer[0]][roomPointer[1]] == 4)
			{
				window.draw(stairs);
				if (player.spr.getPosition().x < GC::ScreenCentre.x + 32 && player.spr.getPosition().x > GC::ScreenCentre.x - 32 && player.spr.getPosition().y > GC::ScreenCentre.y - 32 && player.spr.getPosition().y < GC::ScreenCentre.y + 32)
				{
					score += 100;
					std::cout << "Floor clear\n";
					player.health = player.maxHealth;
					player.spr.setPosition(GC::ScreenCentre.x, GC::ScreenCentre.y);
					roomPointer = { GC::FloorCentre.x, GC::FloorCentre.y };
					++floorNumber;
					if (floorNumber == 5)
					{
						loopbreak = true;
					}
					else
					{
						darkStone -= Color(8, 8, 8, 0);
						lightStone -= Color(8, 8, 8, 0);
						wall.setFillColor(darkStone);
						wall.setOutlineColor(lightStone);
						floorText.setString("Floor: " + std::to_string(floorNumber));
						scoreText.setString(std::to_string(score));
						layout = LG::layoutGeneration(floorNumber);
						wallsCheck(layout, roomPointer, walls);
					}
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
			for (int i = 0; i < player.maxHealth; ++i)
			{
				heartContainer.setPosition(Vector2f(20 + (i * 72), 20));
				window.draw(heartContainer);
				if (player.health > i)
				{
					heart.setPosition(Vector2f(20 + (i * 72), 20));
					window.draw(heart);
				}
			}

			if (jewels[0].found) window.draw(jewels[0].spr);
			if (jewels[1].found) window.draw(jewels[1].spr);
			if (jewels[2].found) window.draw(jewels[2].spr);
			if (jewels[3].found) window.draw(jewels[3].spr);

			window.draw(scoreText);
			window.draw(floorText);
			//The thing that matters
			window.display();
		}
		if (player.health == 0)
		{
			Texture gameOverTex;
			LoadTexture("data/gameover.png", gameOverTex);
			Sprite gameOver;
			gameOver.setTexture(gameOverTex);
			gameOver.setPosition(GC::LowerBounds.x, GC::LowerBounds.y);
			window.draw(gameOver);
			window.display();
			clock.restart();
			float elapsed = 0;
			while (elapsed < 3.f)
			{
				elapsed = clock.getElapsedTime().asSeconds();
			}
			window.close();
		}
		if (floorNumber == 5)
		{
			Texture winTex;
			LoadTexture("data/win.png", winTex);
			Sprite win;
			win.setTexture(winTex);
			win.setPosition(GC::LowerBounds.x, GC::LowerBounds.y);
			window.draw(win);
			window.display();
			clock.restart();
			float elapsed = 0;
			while (elapsed < 5.f)
			{
				elapsed = clock.getElapsedTime().asSeconds();
			}
			window.close();
		}
	}
};