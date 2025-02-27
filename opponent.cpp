#include "opponent.h"
#include "player.h"
#include "physics.h"
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

std::vector<CollisionBox> CommaHitboxes{{{{-30,-30},{30,-30},{30,30},{-30,30}}}};
std::vector<CollisionBox> PeriodHitboxes{{{{-140,-10},{140,-10},{140,10},{-140,10}}}};
std::vector<CollisionBox> ReturnHitboxes{CollisionBox{{{-5,-200},{-5,200},{5,200},{5,-200}}}, 
                                        CollisionBox{{{-10,200},{0,250},{-10,200}}}};

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

float truncatedNormal(std::normal_distribution<float>& normal, std::mt19937& gen, float min, float max, float offset=0)
{
    float out = normal(gen) + offset;
    while(out < min || out > max)
    {
        out = normal(gen) + offset;
    }
    return out;
}

void Opponent::tick()
{
    energy += 1.f;
    if(delay > 0) 
    {
        delay -= 1.f;
        return;
    }
    if(energy < 0) return;
    Point newProjPos{0,0};
    Vector newProjVelocity{10.f,10.f};
    Vector newProjAcceleration{0.f, 0.f};
    bool right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    bool left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    bool up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    if(right)
    {
        newProjPos.x = game->region.start.x - 200.f;
        newProjPos.y = truncatedNormal(normal, gen, player->border.start.y, player->border.end.y, player->pos.y);
        //newProjVelocity.x *= 1.f;
        newProjVelocity.y = 0.f;
    }
    else if(left)
    {
        newProjPos.x = game->region.end.x + 200.f;
        newProjPos.y = truncatedNormal(normal, gen, player->border.start.y, player->border.end.y, player->pos.y);
        newProjVelocity.x *= -1.f;
        newProjVelocity.y = 0.f;
    }
    else if(down)
    {
        newProjPos.y = game->region.start.y - 200.f;
        newProjPos.x = truncatedNormal(normal, gen, player->border.start.x, player->border.end.x, player->pos.x);
        newProjVelocity.x = 0.f;
        //newProjVelocity.y *= 1.f;
    }
    else if(up)
    {
        newProjPos.y = game->region.end.y + 200.f;
        newProjPos.x = truncatedNormal(normal, gen, player->border.start.x, player->border.end.x, player->pos.x);
        newProjVelocity.x = 0.f;
        newProjVelocity.y *= -1.f;
    }
    else return;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
    {
        energy -= 100.f;
        delay += 50.f;
        game->projectiles.push_back(Projectile{newProjPos, newProjVelocity*1.5f, newProjAcceleration, CommaHitboxes, 0.f, 0.f, -1, sf::Color::White, 600});
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
    {
        energy -= 200.f;
        delay += 100.f;
        game->projectiles.push_back(Projectile{newProjPos, newProjVelocity, newProjAcceleration, PeriodHitboxes, 45.f, 2.f, -1, sf::Color::White, 1000});
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Slash))
    {
        energy -= 200.f;
        delay += 100.f;
        std::vector<CollisionBox> SlashHitboxes{{}};
        if(left || right)
        {
            SlashHitboxes = std::vector<CollisionBox>{{{{-5,player->border.start.y},{-5,player->border.end.y},
                                                        {5,player->border.end.y},{5,player->border.start.y}}}};
            newProjPos.y = 0;
        }
        else if(up || down)
        {
            SlashHitboxes = std::vector<CollisionBox>{{{{player->border.start.x,-5},{player->border.end.x,-5},
                                                        {player->border.end.x,5},{player->border.start.y,5}}}};
            newProjPos.x = 0;
        }
        game->projectiles.push_back(Projectile{newProjPos, newProjVelocity*0.5f, newProjAcceleration, SlashHitboxes, 0.f, 0.f, -1, sf::Color::Blue, 2000});
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
        energy -= 300.f;
        delay += 200.f;
        newProjAcceleration.x = newProjVelocity.x;
        newProjAcceleration.y = newProjVelocity.y;
        newProjAcceleration = newProjAcceleration;
        float angle = 0.f;
        if(left) 
        {
            angle = 90.f;
            newProjPos.y = player->pos.y;
        }
        if(right) 
        {
            angle = 270.f;
            newProjPos.y = player->pos.y;
        }
        if(down) 
        {
            angle = 180.f;
            newProjPos.x = player->pos.x;
        }
        if(up) newProjPos.x = player->pos.x;
        game->projectiles.push_back(Projectile{newProjPos, newProjVelocity*2.f, newProjAcceleration*-0.04f, ReturnHitboxes, angle, 0.f, -1, sf::Color::White, 600});
    }
    //else if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSlash))
}