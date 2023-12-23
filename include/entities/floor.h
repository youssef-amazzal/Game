#pragma once
#include "raylib.h"
#include "assets.h"
#include "entity.h"

#define FLOORS_COUNT 1
typedef enum FLOORS {
    F_NORMAL = 1
} FLOORS;

typedef enum FLOORS_INDEX {
    F_NORMAL_INDEX = 0
} FLOORS_INDEX;

static const TextureData FLOORS_DATA[] = {
    {F_NORMAL, {0, 0, TILE_SIZE * SCALING_FACTOR, TILE_SIZE * SCALING_FACTOR}}
};

typedef struct Floor {
    Entity *entity;
    FLOORS type;
} Floor;

void InitFloors();
Floor *GetSigletonFloor(FLOORS f_type);

