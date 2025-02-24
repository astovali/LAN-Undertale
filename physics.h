#include <vector>
#include <SFML/Graphics.hpp>

struct Point
{
    float x;
    float y;
};


struct Vector
{
    float x;
    float y;
};

struct Region
{
    Point start;
    Point end;
};


struct CollisionBox
{
    std::vector<Point> points;
    Point center;
    float radius;
    CollisionBox(std::vector<Point> _points);
    bool isColliding(CollisionBox other, Point pos, Point otherPos, bool inverseChecked=false);
};


struct Projectile
{
    Point pos;
    Vector velocity;
    std::vector<CollisionBox> hitboxes;
    int renderType = -1;
    sf::Color color = sf::Color::White;
    void tick();
};

struct QuadtreeNode
{
    Region region;
    std::vector<Projectile*> projectiles;
};
struct Collision
{
    Projectile* first;
    Projectile* second;    
};


struct Game
{
    Region region;
    std::vector<Projectile> projectiles;
};


std::vector<Collision> getCollisions(QuadtreeNode node, int depth = 0);