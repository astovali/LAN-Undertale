#include "renderer.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Undertale");
    renderer(window);
    return 0;
}