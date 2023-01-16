#include "SFML/Graphics.hpp"

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(512, 640), "The Sunken Pyramid");

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

	}
}