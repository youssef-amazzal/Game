#pragma once
#include "entity.h"

typedef struct Player
{
    Entity *entity;
} Player;

Player *GetPlayer();