#include "opponent.h"
#include "player.h"
#include "physics.h"
#include <random>
#include <iostream>

std::vector<CollisionBox> CommaHitboxes{{{{-30,-30},{30,-30},{30,30},{-30,30}}}};
std::vector<CollisionBox> PeriodHitboxes{{{{-140,-10},{140,-10},{140,10},{-140,10}}}};

Opponent::Opponent(Game* _game, Player* _player)
{
    energy = 0.f;
    delay = 0.f;
    game = _game;
    player = _player;
    gen = std::mt19937{rd()};
    normal = std::normal_distribution<float>{0.f, 
                                    (player->border.start.x+player->border.end.x)/10.f};
}

void Opponent::tick()
{
    energy += 1.f;
    if(delay > 0) 
    {
        delay -= 1.f;
        return;
    }
    Point newProjPos{0,0};
    Vector newProjVelocity{1.f,1.f};
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        newProjPos.x = game->region.start.x - 50.f;
        float newY = normal(gen) + player->pos.y;
        while(newY < player->border.start.y || newY > player->border.end.y)
        {
            newY = normal(gen) + player->pos.y;
        }
        newProjPos.y = newY;
        //newProjVelocity.x *= 1.f;
        newProjVelocity.y = 0.f;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        newProjPos.x = game->region.end.x + 50.f;
        float newY = normal(gen) + player->pos.y;
        while(newY < player->border.start.y || newY > player->border.end.y)
        {
            newY = normal(gen) + player->pos.y;
        }
        newProjPos.y = newY;
        newProjVelocity.x *= -1.f;
        newProjVelocity.y = 0.f;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        newProjPos.y = game->region.start.y - 50.f;
        float newX = normal(gen) + player->pos.x;
        while(newX < player->border.start.x || newX > player->border.end.x)
        {
            newX = normal(gen) + player->pos.x;
        }
        newProjPos.x = newX;
        newProjVelocity.x = 0.f;
        //newProjVelocity.y *= 1.f;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        newProjPos.y = game->region.end.y + 50.f;
        float newX = normal(gen) + player->pos.x;
        while(newX < player->border.start.x || newX > player->border.end.x)
        {
            newX = normal(gen) + player->pos.x;
        }
        newProjPos.x = newX;
        newProjVelocity.x = 0.f;
        newProjVelocity.y *= -1.f;
    }
    else return;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Comma) && energy > 300.f)
    {
        energy -= 300.f;
        delay += 200.f;
        game->projectiles.push_back(Projectile{newProjPos, newProjVelocity, CommaHitboxes});
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Period) && energy > 1000.f)
    {
        energy -= 1000.f;
        delay += 800.f;
        game->projectiles.push_back(Projectile{newProjPos, newProjVelocity, PeriodHitboxes, 45.f, 0.0001f});
    }
    //else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Slash))
    //else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    //else if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash))
}