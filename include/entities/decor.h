#pragma once
#include "raylib.h"
#include "assets.h"
#include "entity.h"

#define DECORS_COUNT 2
typedef enum DECORS {
    D_BOX_TOP = 12,
    D_BOX_BODY = 21,
    K_KEY = 1,
} DECORS;

typedef enum DECORS_INDEX {
    D_BOX_TOP_INDEX = 0,
    D_BOX_BODY_INDEX = 1,
    K_KEY_INDEX = 2,
} DECORS_INDEX;

typedef struct Decor {
    Entity *entity;
    DECORS type;
} Decor;

void InitDecors();