#include <assert.h>

#include "SFML/Graphics.hpp"


using namespace sf;
using namespace std;

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
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			if (pos.y > (screenSize.y * 0.05f))
			velocity.y -= SPEED;
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			if (pos.y < (screenSize.y * 0.95f))
			velocity.y += SPEED;
		}

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			if (pos.x > (screenSize.x * 0.05f))
			velocity.x -= SPEED;
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			if (pos.x < (screenSize.x * 0.95f))
			velocity.x += SPEED;
		};
		pos += velocity * elapsedSec;
		spr.setPosition(pos);
		/*if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			fire = 1;
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
		spr.setPosition(256, 384);
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

	RenderWindow window(VideoMode(512, 640), "The Sunken Pyramid");

	vector<Object>objects;

	Texture playerTex;
	LoadTexture("data/player.png", playerTex);
	Object player;
	player.type = Object::ObjT::Player;
	player.Init(window, playerTex);
	objects.push_back(player);

	RectangleShape infoRectangle(Vector2f(512.f, 128.f));
	infoRectangle.setFillColor(Color(128, 128, 128));
	
	Clock clock;

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
				if (event.text.unicode == 27) {
					window.close();
				}
			}
		}

		window.clear();

		float elapsed = clock.getElapsedTime().asSeconds();
		clock.restart();

		for (size_t i = 0; i < objects.size(); ++i) //ship vs asteroid
		{
			objects[i].Update(window.getSize(), elapsed);	
		}

		window.draw(infoRectangle);

		for (size_t i = 0; i < objects.size(); ++i) {
			objects[i].Render(window);
		}

		window.display();
	}
}