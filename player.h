#pragma once
#include "physics.h"

struct Player
{
    Point pos;
    CollisionBox hurtbox;
    Region border;
    bool moved = false;
    void tick();
};