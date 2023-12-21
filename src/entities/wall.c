#include "wall.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

static Wall SingletonRecord[WALLS_COUNT];

static void render(Entity *wallEnt);
static void Free(Entity *wallEnt);
static WALLS IndexToType(int index);
static WALLS_INDEX TypeToIndex(WALLS type);

void InitWalls() {
    static bool initialized = false;
    if (!initialized)
    {
        for (int i = 0; i < WALLS_COUNT; i++)
        {
            Wall *wall = &SingletonRecord[i];
            wall->type = IndexToType(i);

            wall->entity = CreateEntity();
            wall->entity->child = &SingletonRecord[i];

            wall->entity->spriteSheet = wallSpriteSheet; // todo: i should change this if there is more wall types
            wall->entity->frameTexture = (Rectangle){TILE_SIZE * (wall->type % W_TILES_PER_ROW), TILE_SIZE * (wall->type / W_TILES_PER_ROW), TILE_SIZE, TILE_SIZE};
            
            wall->entity->Render = render;
            wall->entity->Free = Free;
        }
        initialized = true;
    }
}

Wall *GetSigletonWall(WALLS w_type)
{
    return &SingletonRecord[TypeToIndex(w_type)];
}

/************************
 *                      *
 * Core Funcs           *
 *                      *
 ************************/

static void render(Entity *wallEnt)
{
    Wall *wall = (Wall *)wallEnt->child;

    int width = CURRENT_LEVEL->width;
    int height = CURRENT_LEVEL->height;

    int **layerArray = CURRENT_LEVEL->layers[LAYER_WALLS];

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++)
        {
            if (layerArray[row][col] == wall->type) {
                wallEnt->destFrame.x = col * TILE_SIZE * SCALING_FACTOR;
                wallEnt->destFrame.y = row * TILE_SIZE * SCALING_FACTOR;
                DrawTexturePro(*wallEnt->spriteSheet, wallEnt->frameTexture, wallEnt->destFrame, (Vector2){0, 0}, 0, WHITE);
            }
        }
    }
}

static void Free(Entity *wallEnt)
{
    int id = wallEnt->id;
    Wall *wall = (Wall *)wallEnt->child;

    free(wall);
    free(wallEnt);

    ENTITY_RECORD[id] = NULL;
    ENTITY_COUNT--;
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