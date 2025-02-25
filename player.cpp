#include "player.h"
#include <SFML/Window.hpp>
#include <cmath>

float inverseRoot2 = 1/sqrt(2);
void Player::tick()
{
    Vector movement{0,0};
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= 1.f;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += 1.f;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement.y -= 1.f;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement.y += 1.f;
    }
    if(abs(movement.x)+abs(movement.y) == 2)
    {
        movement.x = movement.x*inverseRoot2;
        movement.y = movement.y*inverseRoot2;
    }
    pos.x += movement.x;
    pos.y += movement.y;
    if(pos.x < border.start.x) pos.x 
}