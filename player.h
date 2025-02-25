#pragma once
#include "physics.h"

struct Player
{
    Point pos;
    CollisionBox hurtbox;
    Region border;
    void tick();
};