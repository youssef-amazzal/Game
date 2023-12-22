#include "wall.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

static const AssetsData COLLISION_DATA[] = {
    {
        .tileTypeId = W_CEILING, 
        .collisionRect = {
            0,
            0,
            0, 
            0
        },
        .zIndex = 1
    },
    {
        .tileTypeId = W_CORNER_TOP_LEFT,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        }
    },
    {
        .tileTypeId = W_CORNER_TOP_RIGHT,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        }
    },
    {
        .tileTypeId = W_CORNER_BOTTOM_LEFT,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        }
    },
    {
        .tileTypeId = W_CORNER_BOTTOM_RIGHT,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        },
    },
    {
        .tileTypeId = W_SIDE_LEFT,
        .collisionRect = {
            TILE_SIZE - (TILE_SIZE / 4),
            0,
            TILE_SIZE / 4,
            TILE_SIZE,
        },
    },
    {
        .tileTypeId = W_SIDE_RIGHT,
        .collisionRect = {
            0,
            0,
            TILE_SIZE / 4,
            TILE_SIZE,
        },
    },
    {
        .tileTypeId = W_SIDE_CENTER,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        },
    },
    {
        .tileTypeId = W_INTERIOR,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        }
    },
    {
        .tileTypeId = W_EXTERIOR,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        }
    },
    {
        .tileTypeId = W_T_TOP,
        .collisionRect = {
            0,
            0,
            TILE_SIZE,
            TILE_SIZE,
        }
    },
    {
        .tileTypeId = W_T_BOTTOM,
        .collisionRect = {
            0 + (float) TILE_SIZE / 3,
            0,
            (float) TILE_SIZE / 3,
            TILE_SIZE,
        }
    }
};

static Wall *GetWall(WALLS w_type);
static void Free(Entity *wallEnt);
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
                    wall->entity->destFrame.x = col * TILE_SIZE * SCALING_FACTOR;
                    wall->entity->destFrame.y = row * TILE_SIZE * SCALING_FACTOR;

                    AssetsData assetsData = COLLISION_DATA[TypeToIndex(layerArray[row][col])];
                    wall->entity->collisionRect.width = assetsData.collisionRect.width * SCALING_FACTOR;
                    wall->entity->collisionRect.height = assetsData.collisionRect.height * SCALING_FACTOR;
                    wall->entity->collisionRect.x = assetsData.collisionRect.x + wall->entity->destFrame.x;
                    wall->entity->collisionRect.y = assetsData.collisionRect.y + wall->entity->destFrame.y;

                    wall->entity->zIndex = assetsData.zIndex;
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