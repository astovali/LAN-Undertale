#include "player.h"
#include <SFML/Window.hpp>
#include <cmath>

float inverseRoot2 = 1/sqrt(2);
float speed = 10.f;
void Player::tick()
{
    Vector movement{0,0};
    moved = false;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        movement.x -= 1.f;
        moved = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        movement.x += 1.f;
        moved = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        movement.y -= 1.f;
        moved = true;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        movement.y += 1.f;
        moved = true;
    }
    if(abs(movement.x)+abs(movement.y) == 2)
    {
        movement.x = movement.x*inverseRoot2;
        movement.y = movement.y*inverseRoot2;
    }
    pos.x += movement.x*speed;
    pos.y += movement.y*speed;
    for(Point& p: hurtbox.points)
    {
        if(p.x+pos.x < border.start.x) pos.x = border.start.x-p.x;
        if(p.x+pos.x > border.end.x) pos.x = border.end.x-p.x;
        if(p.y+pos.y < border.start.y) pos.y = border.start.y-p.y;
        if(p.y+pos.y > border.end.y) pos.y = border.end.y-p.y;
    }
}