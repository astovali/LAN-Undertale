#pragma once
#include "player.h"
#include "physics.h"
#include <random>

struct Opponent
{
    float energy;
    float delay;
    Game* game;
    Player* player;
    std::random_device rd;
    std::mt19937 gen;
    std::normal_distribution<float> normal;
    void tick();
    Opponent(Game* game, Player* player);
};