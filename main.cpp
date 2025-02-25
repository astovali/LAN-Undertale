#include "physics.h"
#include "player.h"
#include "opponent.h"
#include <SFML/Graphics.hpp>
#include <iostream>


int main()
{
    Game game{{{0, 0}, {800, 800}}, {}};
    Player player{{0,0},{{{0,0},{30,0},{30,30},{0,30}}},{{100,100},{700,700}}};
    Opponent opponent{&game, &player};
    
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

        opponent.tick();
        player.tick();
        std::vector<sf::Vertex> vertices;
        vertices.push_back({{player.hurtbox.center.x+player.pos.x, player.hurtbox.center.y+player.pos.y}, sf::Color::Green});
        for(int j=0; j<player.hurtbox.points.size(); j++)
        {
            vertices.push_back({{player.hurtbox.points[j].x+player.pos.x, player.hurtbox.points[j].y+player.pos.y}, sf::Color::Blue});
        }
        vertices.push_back({{player.hurtbox.points[0].x+player.pos.x, player.hurtbox.points[0].y+player.pos.y}, sf::Color::Blue});

        window.draw(&vertices[0], vertices.size(), sf::TriangleFan);

        vertices = {sf::Vertex{{player.border.start.x, player.border.start.y}},
                    sf::Vertex{{player.border.start.x, player.border.end.y}},
                    sf::Vertex{{player.border.end.x, player.border.end.y}},
                    sf::Vertex{{player.border.end.x, player.border.start.y}},
                    sf::Vertex{{player.border.start.x, player.border.start.y}}};
        
        window.draw(&vertices[0], vertices.size(), sf::LineStrip);

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
            for(CollisionBox& box: proj.hitboxes)
            {
                if(player.hurtbox.isColliding(box, player.pos, 0.f, proj.pos, proj.angle))
                {
                    window.close();
                }
            }
            if(proj.renderType == -1)
            {
                for(CollisionBox& box: proj.hitboxes)
                {
                    std::vector<sf::Vertex> vertices;
                    Point p = box.center.rotatedBy(proj.angle).offsetBy(proj.pos);
                    vertices.push_back({{p.x, p.y}, proj.color});
                    for(int j=0; j<box.points.size(); j++)
                    {
                        p = box.points[j].rotatedBy(proj.angle).offsetBy(proj.pos);
                        vertices.push_back({{p.x, p.y}, proj.color});
                    }
                    p = box.points[0].rotatedBy(proj.angle).offsetBy(proj.pos);
                    vertices.push_back({{p.x, p.y}, proj.color});

                    window.draw(&vertices[0], vertices.size(), sf::TriangleFan);
                }
            }
        }
        
        window.display();
    }
    return 0;
}