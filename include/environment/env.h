#pragma once
#include "entity.h"


typedef struct Map {
    int width;
    int height;
} Map;

int **LoadMapArray(char *path, Map level);

