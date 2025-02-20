#include "renderer.h"
#include <SFML/Graphics.hpp>

void renderer(sf::RenderWindow& window)
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);

        window.display();
    }
}