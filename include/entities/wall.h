#pragma once
#include "raylib.h"
#include "assets.h"
#include "entity.h"

#define WALLS_COUNT 12
typedef enum WALLS {
    W_CEILING = 6,
    W_CORNER_TOP_LEFT = 78,
    W_CORNER_TOP_RIGHT = 77,
    W_CORNER_BOTTOM_LEFT = 86,
    W_CORNER_BOTTOM_RIGHT = 87,
    W_SIDE_LEFT = 102,
    W_SIDE_RIGHT = 101,
    W_SIDE_CENTER = 88,
    W_INTERIOR = 15,
    W_EXTERIOR = 51,
    W_T_TOP = 79, // T-shape wall
    W_T_BOTTOM = 97
} WALLS;

typedef enum WALLS_INDEX {
    W_CEILING_INDEX = 0,
    W_CORNER_TOP_LEFT_INDEX = 1,
    W_CORNER_TOP_RIGHT_INDEX = 2,
    W_CORNER_BOTTOM_LEFT_INDEX = 3,
    W_CORNER_BOTTOM_RIGHT_INDEX = 4,
    W_SIDE_LEFT_INDEX = 5,
    W_SIDE_RIGHT_INDEX = 6,
    W_SIDE_CENTER_INDEX = 7,
    W_INTERIOR_INDEX = 8,
    W_EXTERIOR_INDEX = 9,
    W_T_TOP_INDEX = 10,
    W_T_BOTTOM_INDEX = 11
} WALLS_INDEX;

typedef struct Wall {
    Entity *entity;
    WALLS type;
} Wall;

void InitWalls();
Wall *GetSigletonWall(WALLS w_type);