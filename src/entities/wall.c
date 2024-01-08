#include "wall.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

static const TextureData TEXTURES_DATA[] = {
    {
        .tileTypeId = W_CEILING, 
        .hitBox = {
            .area = {
                0,
                0,
                0, 
                0
            },
            .color = BLUE,
        },
        .zIndex = 1
    },
    {
        .tileTypeId = W_CORNER_TOP_LEFT,
        .hitBox = {
            .area = {
                TILE_SIZE / 2  - TILE_SIZE * 0.4,
                TILE_SIZE / 2 - TILE_SIZE / 4,
                TILE_SIZE * 0.8,
                TILE_SIZE / 2.5,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
    },
    {
        .tileTypeId = W_CORNER_TOP_RIGHT,
        .hitBox = {
            .area = {
                TILE_SIZE / 2  - TILE_SIZE * 0.4,
                TILE_SIZE / 2 - TILE_SIZE / 4,
                TILE_SIZE * 0.8,
                TILE_SIZE / 2.5,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
        .zIndex = 1
    },
    {
        .tileTypeId = W_CORNER_BOTTOM_LEFT,
        .hitBox = {
            .area = {
                0,
                0,
                TILE_SIZE / 4,
                TILE_SIZE,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
        .zIndex = 1
    },
    {
        .tileTypeId = W_CORNER_BOTTOM_RIGHT,
        .hitBox = {
            .area = {
                TILE_SIZE - TILE_SIZE / 3,
                0,
                TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
    },
    {
        .tileTypeId = W_SIDE_LEFT,
        .hitBox = {
            .area = {
                0,
                0,
                TILE_SIZE / 4,
                TILE_SIZE,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
        .zIndex = 1
    },
    {
        .tileTypeId = W_SIDE_RIGHT,
        .hitBox = {
            .area = {
                TILE_SIZE - TILE_SIZE / 3,
                0,
                TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
        .zIndex = 1
    },
    {
        .tileTypeId = W_SIDE_CENTER,
        .hitBox = {
            .area = {
                TILE_SIZE / 3,
                0,
                TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
        .zIndex = 1
    },
    {
        .tileTypeId = W_INTERIOR,
        .hitBox = {
            .area = {
                TILE_SIZE / 2  - TILE_SIZE * 0.4,
                TILE_SIZE / 2 - TILE_SIZE / 4,
                TILE_SIZE * 0.8,
                TILE_SIZE / 2.5,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        }
    },
    {
        .tileTypeId = W_EXTERIOR,
        .hitBox = {
            .area = {
                TILE_SIZE / 2  - TILE_SIZE * 0.4,
                TILE_SIZE / 2 - TILE_SIZE / 4,
                TILE_SIZE * 0.8,
                TILE_SIZE / 2.5,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        }
    },
    {
        .tileTypeId = W_T_TOP,
        .hitBox = {
            .area = {
                0,
                TILE_SIZE / 2 - TILE_SIZE / 4,
                TILE_SIZE,
                TILE_SIZE / 2.5,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        }
    },
    {
        .tileTypeId = W_T_BOTTOM,
        .hitBox = {
            .area = {
                0 + (float) TILE_SIZE / 3,
                0,
                (float) TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
            .canBlock = true,
            .canBounce = true,
        },
        .zIndex = 1
    }
};

static Wall *GetWall(WALLS w_type);
static void Free(Entity *wallEnt);
static void SetDestination(Entity *wallEnt, float x, float y);
static WALLS IndexToType(int index);
static WALLS_INDEX TypeToIndex(WALLS type);

void InitWalls() {
    int width = CURRENT_LEVEL->width;
    int height = CURRENT_LEVEL->height;

    int **layerArray = CURRENT_LEVEL->layers[LAYER_WALLS];

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++)
        {
            if (layerArray[row][col] != -1) {
                Wall *wall = GetWall(layerArray[row][col]);
                TextureData textureData = TEXTURES_DATA[TypeToIndex(layerArray[row][col])];
                
                wall->entity->hitBox.area.width = textureData.hitBox.area.width * SCALING_FACTOR;
                wall->entity->hitBox.area.height = textureData.hitBox.area.height * SCALING_FACTOR;
                wall->entity->hitBox.color = textureData.hitBox.color;

                wall->entity->hitBox.canBlock = textureData.hitBox.canBlock;
                wall->entity->hitBox.canBounce = textureData.hitBox.canBounce;

                
                wall->entity->zIndex = textureData.zIndex;

                SetDestination(wall->entity, col * TILE_SIZE * SCALING_FACTOR, row * TILE_SIZE * SCALING_FACTOR);
            }
        }
    }
}

static Wall *GetWall(WALLS type)
{
    Wall *wall = malloc(sizeof(Wall));
    wall->type = type;

    wall->entity = CreateEntity();
    wall->entity->child = wall;

    wall->entity->spriteSheet = wallSpriteSheet;
    wall->entity->frameTexture = (Rectangle){TILE_SIZE * (wall->type % W_TILES_PER_ROW), TILE_SIZE * (wall->type / W_TILES_PER_ROW), TILE_SIZE, TILE_SIZE};

    wall->entity->SetDestination = SetDestination;

    return wall;
}

/************************
 * Core Funcs  
 ************************/

static void Free(Entity *wallEnt)
{
    int id = wallEnt->id;
    Wall *wall = (Wall *)wallEnt->child;

    free(wall);
    free(wallEnt);

    ENTITY_RECORD[id] = NULL;
}

/************************
 * Helper Funcs
 ************************/ 

static void SetDestination(Entity *wallEnt, float x, float y)
{
    Wall *wall = (Wall *)wallEnt->child;
    TextureData textureData = TEXTURES_DATA[TypeToIndex(wall->type)];

    wallEnt->destFrame.x = x;
    wallEnt->destFrame.y = y;
    
    float newHitBoxX = textureData.hitBox.area.x * SCALING_FACTOR + wallEnt->destFrame.x;
    float newHitBoxY = textureData.hitBox.area.y * SCALING_FACTOR + wallEnt->destFrame.y;

    wall->entity->hitBox.area.x = newHitBoxX;
    wall->entity->hitBox.area.y = newHitBoxY;
    
}

static WALLS IndexToType(int index) {
    switch (index)
    {
        case W_CEILING_INDEX:
            return W_CEILING;
        case W_CORNER_TOP_LEFT_INDEX:
            return W_CORNER_TOP_LEFT;
        case W_CORNER_TOP_RIGHT_INDEX:
            return W_CORNER_TOP_RIGHT;
        case W_CORNER_BOTTOM_LEFT_INDEX:
            return W_CORNER_BOTTOM_LEFT;
        case W_CORNER_BOTTOM_RIGHT_INDEX:
            return W_CORNER_BOTTOM_RIGHT;
        case W_SIDE_LEFT_INDEX:
            return W_SIDE_LEFT;
        case W_SIDE_RIGHT_INDEX:
            return W_SIDE_RIGHT;
        case W_SIDE_CENTER_INDEX:
            return W_SIDE_CENTER;
        case W_INTERIOR_INDEX:
            return W_INTERIOR;
        case W_EXTERIOR_INDEX:
            return W_EXTERIOR;
        case W_T_TOP_INDEX:
            return W_T_TOP;
        case W_T_BOTTOM_INDEX:
            return W_T_BOTTOM;
    }
}

static WALLS_INDEX TypeToIndex(WALLS type)
{
    switch (type)
    {
        case W_CEILING:
            return W_CEILING_INDEX;
        case W_CORNER_TOP_LEFT:
            return W_CORNER_TOP_LEFT_INDEX;
        case W_CORNER_TOP_RIGHT:
            return W_CORNER_TOP_RIGHT_INDEX;
        case W_CORNER_BOTTOM_LEFT:
            return W_CORNER_BOTTOM_LEFT_INDEX;
        case W_CORNER_BOTTOM_RIGHT:
            return W_CORNER_BOTTOM_RIGHT_INDEX;
        case W_SIDE_LEFT:
            return W_SIDE_LEFT_INDEX;
        case W_SIDE_RIGHT:
            return W_SIDE_RIGHT_INDEX;
        case W_SIDE_CENTER:
            return W_SIDE_CENTER_INDEX;
        case W_INTERIOR:
            return W_INTERIOR_INDEX;
        case W_EXTERIOR:
            return W_EXTERIOR_INDEX;
        case W_T_TOP:
            return W_T_TOP_INDEX;
        case W_T_BOTTOM:
            return W_T_BOTTOM_INDEX;
    }
}