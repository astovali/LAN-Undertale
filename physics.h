#include <vector>

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
    void tick();
};


struct Game
{
    int width;
    int height;
    std::vector<Projectile> projectiles;
};