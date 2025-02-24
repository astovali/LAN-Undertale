#include "physics.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    CollisionBox aBox{{{0,0},{5,5},{5,0}}};
    Game game{{{0, 0}, {800, 800}}, {}};
    for(float i=0; i<800; i+=6)
    {
        Projectile aProj{{i,0}, {0,i*0.003f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{0,i}, {i*0.003f,0},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{i,800}, {0,i*-0.003f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{800,i}, {i*-0.003f,0},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{i,30}, {0,i*0.002f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{30,i}, {i*0.002f,0},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{i,770}, {0,i*-0.002f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{770,i}, {i*-0.002f,0},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{i,60}, {0,i*0.001f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{60,i}, {i*0.001f,0},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{i,740}, {0,i*-0.001f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{740,i}, {i*-0.001f,0},{aBox}};
        game.projectiles.push_back(aProj);
    }
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