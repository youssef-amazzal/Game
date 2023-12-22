#include "wall.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

static const AssetsData COLLISION_DATA[] = {
    {
        .tileTypeId = W_CEILING, 
        .collision = {
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
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE,
                TILE_SIZE,
            },
            .color = BLUE,
        }
    },
    {
        .tileTypeId = W_CORNER_TOP_RIGHT,
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE,
                TILE_SIZE,
            },
            .color = BLUE,
        }
    },
    {
        .tileTypeId = W_CORNER_BOTTOM_LEFT,
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE / 4,
                TILE_SIZE,
            },
            .color = BLUE,
        }
    },
    {
        .tileTypeId = W_CORNER_BOTTOM_RIGHT,
        .collision = {
            .area = {
                TILE_SIZE - TILE_SIZE / 3,
                0,
                TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
        },
    },
    {
        .tileTypeId = W_SIDE_LEFT,
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE / 4,
                TILE_SIZE,
            },
            .color = BLUE,
        },
    },
    {
        .tileTypeId = W_SIDE_RIGHT,
        .collision = {
            .area = {
                TILE_SIZE - TILE_SIZE / 3,
                0,
                TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
        },
    },
    {
        .tileTypeId = W_SIDE_CENTER,
        .collision = {
            .area = {
                TILE_SIZE / 3,
                0,
                TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
        },
    },
    {
        .tileTypeId = W_INTERIOR,
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE,
                TILE_SIZE,
            },
            .color = BLUE,
        }
    },
    {
        .tileTypeId = W_EXTERIOR,
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE,
                TILE_SIZE,
            },
            .color = BLUE,
        }
    },
    {
        .tileTypeId = W_T_TOP,
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE,
                TILE_SIZE,
            },
            .color = BLUE,
        }
    },
    {
        .tileTypeId = W_T_BOTTOM,
        .collision = {
            .area = {
                0 + (float) TILE_SIZE / 3,
                0,
                (float) TILE_SIZE / 3,
                TILE_SIZE,
            },
            .color = BLUE,
        }
    }
};

static Wall *GetWall(WALLS w_type);
static void Free(Entity *wallEnt);
static void SetDestination(Entity *wallEnt, float x, float y);
static WALLS IndexToType(int index);
static WALLS_INDEX TypeToIndex(WALLS type);

void InitWalls() {
    static bool initialized = false;
    if (!initialized)
    {
        int width = CURRENT_LEVEL->width;
        int height = CURRENT_LEVEL->height;

        int **layerArray = CURRENT_LEVEL->layers[LAYER_WALLS];

        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++)
            {
                if (layerArray[row][col] != -1) {
                    Wall *wall = GetWall(layerArray[row][col]);
                    AssetsData assetsData = COLLISION_DATA[TypeToIndex(layerArray[row][col])];
                    
                    wall->entity->collision.area.width = assetsData.collision.area.width * SCALING_FACTOR;
                    wall->entity->collision.area.height = assetsData.collision.area.height * SCALING_FACTOR;
                    wall->entity->collision.color = assetsData.collision.color;
                    

                    wall->entity->zIndex = assetsData.zIndex;

                    SetDestination(wall->entity, col * TILE_SIZE * SCALING_FACTOR, row * TILE_SIZE * SCALING_FACTOR);
                }
            }
        }

        initialized = true;
    }
}

static Wall *GetWall(WALLS w_type)
{
    Wall *wall = malloc(sizeof(Wall));
    wall->type = w_type;

    wall->entity = CreateEntity();
    wall->entity->child = wall;

    wall->entity->spriteSheet = wallSpriteSheet;
    wall->entity->frameTexture = (Rectangle){TILE_SIZE * (wall->type % W_TILES_PER_ROW), TILE_SIZE * (wall->type / W_TILES_PER_ROW), TILE_SIZE, TILE_SIZE};

    wall->entity->Free = Free;
    wall->entity->SetDestination = SetDestination;

    return wall;
}

/************************
 *                      *
 * Core Funcs           *
 *                      *
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
 *                      *
 * Helper Funcs         *
 *                      *
 ************************/ 

static void SetDestination(Entity *wallEnt, float x, float y)
{
    Wall *wall = (Wall *)wallEnt->child;
    AssetsData assetsData = COLLISION_DATA[TypeToIndex(wall->type)];

    wallEnt->destFrame.x = x;
    wallEnt->destFrame.y = y;
    
    float newCollisionX = assetsData.collision.area.x * SCALING_FACTOR + wallEnt->destFrame.x;
    float newCollisionY = assetsData.collision.area.y * SCALING_FACTOR + wallEnt->destFrame.y;

    wall->entity->collision.area.x = newCollisionX;
    wall->entity->collision.area.y = newCollisionY;
    
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