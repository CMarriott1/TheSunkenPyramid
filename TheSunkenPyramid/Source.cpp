#include "SFML/Graphics.hpp"

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(512, 640), "The Sunken Pyramid");

	RectangleShape infoRectangle(Vector2f(512.f, 128.f));
	infoRectangle.setFillColor(Color(128, 128, 128));
	

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

		window.draw(infoRectangle);
		window.display();
	}
}