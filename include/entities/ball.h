#pragma once
#include "raylib.h"
#include "assets.h"
#include "entity.h"

#define BALLS_COUNT 2
typedef enum BALLS {
    B_GRAY = 8,
    B_BROWN = 7,
} BALLS;

typedef enum BALLS_INDEX {
    B_GRAY_INDEX = 0,
    B_BROWN_INDEX = 1
} BALLS_INDEX;

typedef struct Ball {
    Entity *entity;
    BALLS type;
} Ball;

void InitBalls();