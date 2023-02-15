#include <assert.h>
#include <string>
#include <iostream>

#include "SFML/Graphics.hpp"
#include "Game.h"

using namespace sf;
using namespace std;

Vector2f Decay(Vector2f& currentVal, float pcnt, float timeInterval, float dTimeS)
{
	float mod = 1.0f - pcnt * (dTimeS / timeInterval);
	Vector2f alpha(currentVal.x * mod, currentVal.y * mod);
	return alpha;
}


bool LoadTexture(const string& file, Texture& tex)
{
	if (tex.loadFromFile(file))
	{
		tex.setSmooth(true);
		return true;
	}
	assert(false);
	return false;
}

bool CircleToCircle(const Vector2f& pos1, const Vector2f& pos2, float minDist)
{
	float dist = (pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y);
	dist = sqrtf(dist);
	return dist <= minDist;
}

struct Object
{

	enum class ObjT { Ship, Rock, Bullet };
	ObjT type;
	Sprite spr;
	int radius;
	bool colliding;
	bool active = false;
	Vector2f velocity = Vector2f(0.f,0.f);
	int health;
	
	

	void PlayerControl(const Vector2u& screenSize, float elapsedSec, bool& fire) {
		Vector2f pos = spr.getPosition();
		const float SPEED = 250.f;
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (pos.y > (screenSize.y * 0.14f))
				if (velocity.y > 0)
					velocity.y = 0;
			else 
				velocity.y = -SPEED;
			
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (pos.y < (screenSize.y * 0.86f))
				if (velocity.y < 0)
					velocity.y = 0;
			else
				velocity.y = SPEED;
		}
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (pos.x > (screenSize.x * 0.1f))
				if (velocity.x > 0)
					velocity.x = 0;
			else
				velocity.x = -SPEED;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (pos.x < (screenSize.x * 0.9f))
				if (velocity.x < 0)
					velocity.x = 0;
			else
				velocity.x = SPEED;
		};
		velocity = Decay(velocity, 0.1f, 0.02f, elapsedSec);
		pos += velocity * elapsedSec;
		spr.setPosition(pos);
	}

	void MoveRock(float elapsed) {
		if (active == true) {
			const Vector2f& pos = spr.getPosition();
			float x = pos.x - 50 * elapsed;
			if (x < -spr.getGlobalBounds().width / 2.f)
				active = false;
			spr.setPosition(x, pos.y);
		}
	}

	void MoveBullet(const Vector2u& screenSize, float elapsedSec) {
		Vector2f pos = spr.getPosition();
		pos.x += 375 * elapsedSec;
		spr.setPosition(pos);
	}

	void Update(const Vector2u& screenSize, float elapsed, bool& fire) {
		if (active == true) {
			spr.setColor(Color(255, 255, 255, 255));
			switch (type)
			{
			case ObjT::Ship:
				PlayerControl(screenSize, elapsed, fire);
				break;
			case ObjT::Rock:
				MoveRock(elapsed);
				break;
			case ObjT::Bullet:
				MoveBullet(screenSize, elapsed);
				break;
			}
		}
		else spr.setColor(Color(0,0,0,0));
	};

	void Render(RenderWindow& window) {
		//CircleShape circle(radius);
		//circle.setOrigin(radius, radius);
		//circle.setFillColor(Color(0, 0, 0, 0));
		//circle.setOutlineThickness(2.f);
		//if (colliding == false) circle.setOutlineColor(sf::Color(100, 255, 100));
		//else circle.setOutlineColor(sf::Color(255, 100, 100));
		//circle.setPosition(spr.getPosition());
		//window.draw(circle);

		window.draw(spr);
	};

	void InitShip(RenderWindow& window, Texture& tex) {
		radius = 30.f;
		spr.setTexture(tex);
		IntRect texR = spr.getTextureRect();
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		spr.setScale(0.1f, 0.1f);
		spr.setRotation(90);
		spr.setPosition(window.getSize().x * 0.05f, window.getSize().y / 2.f);
		active = true;
		health = 3;
	};

	void InitRock(RenderWindow& window, Texture& tex) {
		radius = 15.f + float(rand()%31);
		float scale = (radius  - 5.f) / 35.f;
		spr.setTexture(tex);
		IntRect texR(0, 0, 96, 96);
		spr.setTextureRect(texR);
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		spr.setScale(scale, scale);
		health = (radius - 15) / 6 + 1;
	};

	void InitBullet(RenderWindow& window, Texture& tex){
		spr.setTexture(tex);
		IntRect texR(0, 0, 32, 32);
		spr.setTextureRect(texR);
		spr.setOrigin(texR.width / 2.f, texR.height / 2.f);
		radius = 5.f;
		float scale = 0.5f;
		spr.setScale(scale, scale);
		spr.setRotation(90);
		active = false;
		health = 0;
	}


	void Init(RenderWindow& window, Texture& tex)
	{
		switch (type)
		{
		case ObjT::Ship:
			InitShip(window, tex);
			break;
		case ObjT::Rock:
			InitRock(window, tex);
			break;
		case ObjT::Bullet:
			InitBullet(window, tex);
			break;
		default:
			assert(false);
		}
	}
};

int main()
{

	srand(time(NULL));

	// Create the main window
	RenderWindow window(VideoMode(GC::SCREEN_RES.x, GC::SCREEN_RES.y), "My first code");

	vector<Object>objects;

	Texture shipTex;
	LoadTexture("data/ship.png", shipTex);
	Object ship;
	ship.type = Object::ObjT::Ship;
	ship.Init(window, shipTex);
	objects.push_back(ship);

	Texture rockTex;
	LoadTexture("data/asteroid.png", rockTex);
	Object rock;
	rock.type = Object::ObjT::Rock;
	rock.Init(window, rockTex);
	objects.insert(objects.end(), GC::NUM_ROCKS, rock);
	for (int i = 1; i < GC::NUM_ROCKS + 1; ++i) {
		objects[i].Init(window, rockTex);
		int x = rand() % 704 + 48;
		int y = rand() % 506 + 48;
		objects[i].spr.setPosition(x, y);
	}
	for (int i = 1; i < objects.size(); ++i) {
		if (objects[i].type == Object::ObjT::Rock) {
			objects[i].active = true;
			int loopCount = 0;
			bool colliding = false;
			do {
				colliding = false;
				++loopCount;
				int x = rand() % 704 + 48;
				int y = rand() % 506 + 48;
				objects[i].spr.setPosition(x, y);
				for (int j = 0; j < objects.size(); ++j) {
					if (i != j && objects[j].active == true) {
						if (CircleToCircle(objects[i].spr.getPosition(), objects[j].spr.getPosition(), (objects[i].radius + objects[j].radius + 20))) { //if colliding
							colliding = true;
							break;
						}
					}
				}
			} while (colliding == true && loopCount < 10);
		}
	}

	Texture bulletTex;
	LoadTexture("data/bullet.png", bulletTex);
	Object bullet;
	bullet.type = Object::ObjT::Bullet;
	bullet.Init(window, bulletTex);
	objects.insert(objects.end(), GC::NUM_BULLETS, bullet);

	window.setKeyRepeatEnabled(false);

	Clock clock;

	vector<bool>fireInputCheck{ false,false };

	// Start the game loop 
	while (window.isOpen())
	{
		fireInputCheck[0] = fireInputCheck[1];
		fireInputCheck[1] = false;

		// Process events
		Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
			{
				if (event.text.unicode == GC::ESCAPE_KEY) {
					window.close();
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					fireInputCheck[1] = true;
				}
			}
		}
		// Clear screen
		window.clear();

		float elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();

		bool fire = false;

		for (size_t i = 0; i < objects.size(); ++i)
		{
			objects[i].colliding = false;
		}
		for (size_t i = 0; i < objects.size() - GC::NUM_BULLETS; ++i) //ship vs asteroid
		{
			objects[i].Update(window.getSize(), elapsed, fire);
			if (objects[i].active == true) {
				for (size_t j = 1 + i; j < objects.size() - GC::NUM_BULLETS; ++j)
				{
					if (CircleToCircle(objects[i].spr.getPosition(), objects[j].spr.getPosition(), (objects[i].radius + objects[j].radius)) && objects[j].active == true) { //if colliding
						objects[i].colliding = true;
						objects[j].colliding = true;
						if (objects[i].type != Object::ObjT::Ship) {
							if (objects[i].spr.getPosition().x >= objects[j].spr.getPosition().x) objects[i].active = false;
							else objects[j].active = false;
						}
						else {
							objects[i].health -= 1;
							if (objects[i].health <= 0) objects[i].active = false;
						}
					}
				}
			}
			else if (objects[i].type == Object::ObjT::Rock) {
				objects[i].active = true;
				objects[i].radius += 20;
				objects[i].spr.setPosition(GC::SCREEN_RES.x, objects[i].radius + rand() % (GC::SCREEN_RES.y - 2 * objects[i].radius));
				for (size_t j = 1 + i; j < objects.size(); ++j)
				{
					if (CircleToCircle(objects[i].spr.getPosition(), objects[j].spr.getPosition(), (objects[i].radius + objects[j].radius))) { //if colliding
						objects[i].active = false;
						objects[i].spr.setRotation(90);
						break;
					}
				}
				objects[i].radius -= 20;
			}
		}


		if (fireInputCheck == vector<bool>{false, true})
		{
			for (size_t i = 51; i < objects.size(); ++i) 
			{
				if (objects[i].active == false) 
				{
					objects[i].active = true;
					objects[i].spr.setPosition(objects[0].spr.getPosition());
					break;
				}
			}
		}


		for (size_t i = 1 + GC::NUM_ROCKS; i < objects.size(); ++i) {//bullets
			objects[i].Update(window.getSize(), elapsed, fire);
			if (objects[i].active == true) {
				for (size_t j = 1; j < objects.size() - GC::NUM_BULLETS; ++j) { //asteroids
					if (objects[j].active == true) {
						if (CircleToCircle(objects[i].spr.getPosition(), objects[j].spr.getPosition(), (objects[i].radius + objects[j].radius))) {
							objects[j].active = false;
							objects[i].health -= 1;
							if (objects[i].health <= 0) objects[i].active = false;
							break;

						}
					}
				}
			}
			if (objects[i].spr.getPosition().x > 800) objects[i].active = false;
		}

		for (size_t i = 0; i < objects.size(); ++i) {
			objects[i].Render(window);
		}
		

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}
