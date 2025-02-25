#include "physics.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    Game game{{{0, 0}, {800, 800}}, {}};
    Player player{{0,0},{{{0,0},{30,0},{30,30},{0,30}}}};
    
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Undertale");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        player.tick();
        std::vector<sf::Vertex> vertices;
        vertices.push_back({{player.hurtbox.center.x+player.pos.x, player.hurtbox.center.y+player.pos.y}, sf::Color::Green});
        for(int j=0; j<player.hurtbox.points.size(); j++)
        {
            vertices.push_back({{player.hurtbox.points[j].x+player.pos.x, player.hurtbox.points[j].y+player.pos.y}, sf::Color::Blue});
        }
        vertices.push_back({{player.hurtbox.points[0].x+player.pos.x, player.hurtbox.points[0].y+player.pos.y}, sf::Color::Blue});

        window.draw(&vertices[0], vertices.size(), sf::TriangleFan);

        QuadtreeNode node{game.region, {}};
        node.projectiles.reserve((int)game.projectiles.size());
        for(Projectile& proj: game.projectiles) node.projectiles.push_back(&proj);
        std::vector<Collision> collisions = getCollisions(node);
        /*
        window.close();
        break;
        */
        for(int i=0; i<game.projectiles.size(); i++)
        {
            game.projectiles[i].color = sf::Color::White;
        }
        for(int i=0; i<collisions.size(); i++)
        {
            collisions[i].first->color = sf::Color::Red;
            collisions[i].second->color = sf::Color::Red;
        }
        for(int i=0; i<game.projectiles.size(); i++)
        {
            Projectile& proj = game.projectiles[i];
            proj.tick();
            if(proj.pos.x < game.region.start.x-50
                || proj.pos.y < game.region.start.y-50
                || proj.pos.x > game.region.end.x+50
                || proj.pos.y > game.region.end.y+50)
            {
                game.projectiles.erase(game.projectiles.begin() + i);
                i--;
                continue;
            }
            if(proj.renderType == -1)
            {
                for(CollisionBox& box: proj.hitboxes)
                {
                    std::vector<sf::Vertex> vertices;
                    vertices.push_back({{box.center.x+proj.pos.x, box.center.y+proj.pos.y}, proj.color});
                    for(int j=0; j<box.points.size(); j++)
                    {
                        vertices.push_back({{box.points[j].x+proj.pos.x, box.points[j].y+proj.pos.y}, proj.color});
                    }
                    vertices.push_back({{box.points[0].x+proj.pos.x, box.points[0].y+proj.pos.y}, proj.color});

                    window.draw(&vertices[0], vertices.size(), sf::TriangleFan);
                }
            }
        }
        
        window.display();
    }
    return 0;
}