#include "physics.h"
#include <iostream>
#include <cmath>

bool CollisionBox::isColliding(CollisionBox other, Point pos, Point otherPos, bool inverseChecked)
{
    /*
    float distanceSquared = (pos.x+center.x - (otherPos.x+other.center.x)) * (pos.x+center.x - (otherPos.x+other.center.x))
                    + (pos.y+center.y - (otherPos.y+other.center.y)) * (pos.y+center.y - (otherPos.y+other.center.y));
    float radiiSquared = (radius+other.radius) * (radius+other.radius);
    if(distanceSquared > radiiSquared) return false;
    */

    std::vector<Vector> dividingAxes = {{points[0].x-points[points.size()-1].x, points[0].y-points[points.size()-1].y}};
    for(int i=0; i<points.size()-1; i++)
    {
        dividingAxes.push_back({points[i+1].x-points[i].x, points[i+1].y-points[i].y});
    }

    for(int i=0; i<dividingAxes.size(); i++)
    {
        bool isIntersection = false;
        Vector perpendicular{-dividingAxes[i].y, dividingAxes[i].x};
        float projection = perpendicular.x*(points[0].x+pos.x) 
                        + perpendicular.y*(points[0].y+pos.y);
        float minSelf = projection;
        float maxSelf = projection;
        for(int j=1; j<points.size(); j++)
        {
            float projection = perpendicular.x*(points[j].x+pos.x) 
                            + perpendicular.y*(points[j].y+pos.y);
            if(projection < minSelf) minSelf = projection;
            if(projection > maxSelf) maxSelf = projection;
        }
        for(int j=0; j<other.points.size(); j++)
        {
            float projection = perpendicular.x*(other.points[j].x+otherPos.x) 
                            + perpendicular.y*(other.points[j].y+otherPos.y);
            if(projection >= minSelf && projection <= maxSelf)
            {
                isIntersection = true;
                break;
            }
        }
        if(!isIntersection)
        {
            if(inverseChecked) return false;
            return other.isColliding(*this, otherPos, pos, true);
        };
    }
    if(inverseChecked) return true;
    return other.isColliding(*this, otherPos, pos, true);
}

CollisionBox::CollisionBox(std::vector<Point> _points)
{
    points = _points;
    float sumX = 0;
    float sumY = 0;
    for(int i=0; i<points.size(); i++)
    {
        sumX += points[i].x;
        sumY += points[i].y;
    }
    center = {sumX/points.size(), sumY/points.size()};
    float maxRadius = 0;
    for(int i=0; i<points.size(); i++)
    {
        float tempX = points[i].x-center.x;
        float tempY = points[i].y-center.y;
        float tempRadius = sqrt(tempX*tempX + tempY*tempY);
        if(tempRadius > maxRadius) maxRadius = tempRadius;
    }
    radius = maxRadius;
}

void Projectile::tick()
{
    pos.x += velocity.x;
    pos.y += velocity.y;
}

int quadtreeDivValue = 3;
int quadtreeDepthMax = 100;
std::vector<Collision> getCollisions(QuadtreeNode node, int depth)
{
    if(node.projectiles.size() <= quadtreeDivValue || depth >= quadtreeDepthMax)
    {
        std::vector<Collision> collisions;
        for(int i=0; i<node.projectiles.size(); i++)
        {
            for(int j=0; j<node.projectiles.size(); j++)
            {
                if(i==j) continue;
                bool isCollision = false;
                for(int a=0; a<node.projectiles[i]->hitboxes.size(); a++)
                {
                    for(int b=0; b<node.projectiles[j]->hitboxes.size(); b++)
                    {
                        if(node.projectiles[i]->hitboxes[a].isColliding(node.projectiles[j]->hitboxes[b],
                            node.projectiles[i]->pos, node.projectiles[j]->pos))
                        {
                            isCollision = true;
                            break;
                        }
                    }  
                    if(isCollision) break;
                }
                if(isCollision)
                {
                    Collision collision{node.projectiles[i], node.projectiles[j]};
                    collisions.push_back(collision);
                }
            }
        }
        return collisions;
    }
    Region region = node.region;
    QuadtreeNode q1{{{region.start.x, region.start.y}, {(region.start.x+region.end.x)/2, (region.start.y+region.end.y)/2}}, {}};
    QuadtreeNode q2{{{(region.start.x+region.end.x)/2, region.start.y}, {region.end.x, (region.start.y+region.end.y)/2}}, {}};
    QuadtreeNode q3{{{region.start.x, (region.start.y+region.end.y)/2}, {(region.start.x+region.end.x)/2, region.end.y}}, {}};
    QuadtreeNode q4{{{(region.start.x+region.end.x)/2, (region.start.y+region.end.y)/2}, {region.end.x, region.end.y}}, {}};
    /*
    std::cout << q1.region.start.x << ',' << q1.region.start.y << '~' << q1.region.end.x << ',' << q1.region.end.y << '\n';
    std::cout << q2.region.start.x << ',' << q2.region.start.y << '~' << q2.region.end.x << ',' << q2.region.end.y << '\n';
    std::cout << q3.region.start.x << ',' << q3.region.start.y << '~' << q3.region.end.x << ',' << q3.region.end.y << '\n';
    std::cout << q4.region.start.x << ',' << q4.region.start.y << '~' << q4.region.end.x << ',' << q4.region.end.y << '\n';
    std::cout << '\n';
    */
    for(Projectile* proj: node.projectiles)
    {
        bool inQ1 = false;
        bool inQ2 = false;
        bool inQ3 = false;
        bool inQ4 = false;
        for(CollisionBox& hitbox: proj->hitboxes)
        {
            for(Point& point: hitbox.points)
            {
                Point p{point.x+proj->pos.x, point.y+proj->pos.y};
                if(p.x >= q1.region.start.x && p.y >= q1.region.start.y
                    && p.x <= q1.region.end.x && p.y <= q1.region.end.y) inQ1 = true;
                if(p.x >= q2.region.start.x && p.y >= q2.region.start.y
                    && p.x <= q2.region.end.x && p.y <= q2.region.end.y) inQ2 = true;
                if(p.x >= q3.region.start.x && p.y >= q3.region.start.y
                    && p.x <= q3.region.end.x && p.y <= q3.region.end.y) inQ3 = true;
                if(p.x >= q4.region.start.x && p.y >= q4.region.start.y
                    && p.x <= q4.region.end.x && p.y <= q4.region.end.y) inQ4 = true;
                if(inQ1 && inQ2 && inQ3 && inQ4) break;
            }
            if(inQ1 && inQ2 && inQ3 && inQ4) break;
        }
        if(inQ1) q1.projectiles.push_back(proj);
        if(inQ2) q2.projectiles.push_back(proj);
        if(inQ3) q3.projectiles.push_back(proj);
        if(inQ4) q4.projectiles.push_back(proj);
    }
    std::vector<Collision> collisions;
    //std::cout << "q1:\n";
    std::vector<Collision> q1Collisions = getCollisions(q1, depth + 1);
    //std::cout << "q2:\n";
    std::vector<Collision> q2Collisions = getCollisions(q2, depth + 1);
    //std::cout << "q3:\n";
    std::vector<Collision> q3Collisions = getCollisions(q3, depth + 1);
    //std::cout << "q4:\n";
    std::vector<Collision> q4Collisions = getCollisions(q4, depth + 1);
    collisions.reserve(q1Collisions.size()+q2Collisions.size()+q3Collisions.size()+q4Collisions.size());
    collisions.insert(collisions.end(), q1Collisions.begin(), q1Collisions.end());
    collisions.insert(collisions.end(), q2Collisions.begin(), q2Collisions.end());
    collisions.insert(collisions.end(), q3Collisions.begin(), q3Collisions.end());
    collisions.insert(collisions.end(), q4Collisions.begin(), q4Collisions.end());
    return collisions;
}