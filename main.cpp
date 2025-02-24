#include "physics.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    CollisionBox aBox{{{0,0},{8,15},{15,0}}};
    Game game{800, 800, {}};
    for(float i=0; i<800; i+=20)
    {
        Projectile aProj{{i,0}, {0,i*0.001f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{0,i}, {i*0.001f,0},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{i,800}, {0,i*-0.001f},{aBox}};
        game.projectiles.push_back(aProj);
        aProj = Projectile{{800,i}, {i*-0.001f,0},{aBox}};
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

        for(int i=0; i<game.projectiles.size(); i++)
        {
            bool colliding = false;
            for(int j=0; j<game.projectiles.size(); j++)
            {
                if(i==j) continue;
                if(game.projectiles[i].hitboxes[0].isColliding(game.projectiles[j].hitboxes[0], 
                    game.projectiles[i].pos, game.projectiles[j].pos))
                {
                    colliding = true;
                }
            }
            Projectile& proj = game.projectiles[i];
            proj.tick();
            if(proj.renderType == -1)
            {
                for(CollisionBox& box: proj.hitboxes)
                {
                    std::vector<sf::Vertex> vertices;
                    for(Point& p: box.points)
                    {
                        if(colliding)
                            vertices.push_back({{p.x+proj.pos.x, p.y+proj.pos.y}, sf::Color::Red});
                        else
                            vertices.push_back({{p.x+proj.pos.x, p.y+proj.pos.y}, sf::Color::White});
                    }
                    
                    window.draw(&vertices[0], vertices.size(), sf::TriangleStrip);
                }
            }
        }
        
        window.display();
    }
    return 0;
}