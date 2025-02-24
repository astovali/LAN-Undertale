#include "physics.h"
#include <iostream>
#include <cmath>

bool CollisionBox::isColliding(CollisionBox other, Point pos, Point otherPos, bool inverseChecked)
{
    float distanceSquared = (pos.x+center.x - (otherPos.x+other.center.x)) * (pos.x+center.x - (otherPos.x+other.center.x))
                    + (pos.y+center.y - (otherPos.y+other.center.y)) * (pos.y+center.y - (otherPos.y+other.center.y));
    float radiiSquared = (radius+other.radius) * (radius+other.radius);
    if(distanceSquared > radiiSquared) return false;
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