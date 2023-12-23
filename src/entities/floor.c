#include "floor.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

static Floor SingletonRecord[FLOORS_COUNT];

static void render(Entity *floorEnt);
static void Free(Entity *floorEnt);
static FLOORS IndexToType(int index);
static FLOORS_INDEX TypeToIndex(FLOORS type);

void InitFloors() {
    static bool initialized = false;
    if (!initialized)
    {
        for (int i = 0; i < FLOORS_COUNT; i++)
        {
            Floor *floor = &SingletonRecord[i];
            floor->type = IndexToType(i);

            floor->entity = CreateEntity();
            floor->entity->child = &SingletonRecord[i];

            floor->entity->spriteSheet = floorSpriteSheet; // todo: i should change this if there is more floor types
            floor->entity->frameTexture = (Rectangle){TILE_SIZE * (floor->type % F_TILES_PER_ROW), TILE_SIZE * (floor->type / F_TILES_PER_ROW), TILE_SIZE, TILE_SIZE};

            floor->entity->zIndex = -1;

            floor->entity->Render = render;
            floor->entity->Free = Free;
        }
        initialized = true;
    }
}

Floor *GetSigletonFloor(FLOORS f_type)
{
    return &SingletonRecord[TypeToIndex(f_type)];
}

/************************
 * Core Funcs
 ************************/

static void render(Entity *floorEnt)
{
    Floor *floor = (Floor *)floorEnt->child;

    int width = CURRENT_LEVEL->width;
    int height = CURRENT_LEVEL->height;

    int **layerArray = CURRENT_LEVEL->layers[LAYER_FLOOR];

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++)
        {
            if (layerArray[row][col] == floor->type) {
                floorEnt->destFrame.x = col * TILE_SIZE * SCALING_FACTOR;
                floorEnt->destFrame.y = row * TILE_SIZE * SCALING_FACTOR;
                DrawTexturePro(*(floorEnt->spriteSheet), floorEnt->frameTexture, floorEnt->destFrame, floorEnt->origin, 0, WHITE);
            }
        }
    }
    floorEnt->destFrame.x = 0;
    floorEnt->destFrame.y = 0;
}

static void Free(Entity *floorEnt)
{
    int id = floorEnt->id;
    Floor *floor = (Floor *)floorEnt->child;

    free(floorEnt);
    free(floor);

    ENTITY_RECORD[id] = NULL;
}

/************************
 * Helper Funcs
 ************************/ 
static FLOORS IndexToType(int index) {
    switch (index)
    {
        case F_NORMAL_INDEX:
            return F_NORMAL;
    }
}            

static FLOORS_INDEX TypeToIndex(FLOORS type) {
    switch (type)
    {
        case F_NORMAL:
            return F_NORMAL_INDEX;
    }
}
