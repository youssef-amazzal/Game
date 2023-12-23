#include "decor.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

const TextureData TEXTURES_DATA[] = {
    {
        .tileTypeId = D_BOX_TOP, 
        .collision = {
            .area = {
                0,
                0,
                0,
                0,
            },
            .color = RED,
        },
        .zIndex = 1,
    },
    {
        .tileTypeId = D_BOX_BODY ,
        .collision = {
            .area = {
                0,
                0,
                TILE_SIZE,
                TILE_SIZE,
            },
            .color = RED,
        },
        .isReactive = true,
    },
};

static Decor *GetDecor(DECORS type);
static void Free(Entity *decorEnt);
static void SetDestination(Entity *decorEnt, float x, float y);
static DECORS IndexToType(int index);
static DECORS_INDEX TypeToIndex(DECORS type);

void InitDecors() 
{
    static bool initialized = false;
    if (!initialized) 
    {
        int width = CURRENT_LEVEL->width;
        int height = CURRENT_LEVEL->height;

        int **layerArray = CURRENT_LEVEL->layers[LAYER_DECOR];

        for (int row = 0; row < height; row++) 
        {
            for (int col = 0; col < width; col++)
            {
                if (layerArray[row][col] != -1) 
                {
                    Decor *decor = GetDecor(layerArray[row][col]);
                    TextureData textureData = TEXTURES_DATA[TypeToIndex(layerArray[row][col])];

                    decor->entity->collision.area.width = textureData.collision.area.width * SCALING_FACTOR;
                    decor->entity->collision.area.height = textureData.collision.area.height * SCALING_FACTOR;
                    decor->entity->collision.color = textureData.collision.color;

                    decor->entity->isReactive = textureData.isReactive;                    
                    decor->entity->zIndex = textureData.zIndex;

                    SetDestination(decor->entity, col * TILE_SIZE * SCALING_FACTOR, row * TILE_SIZE * SCALING_FACTOR);
                }
            }
        }
        
        initialized = true;
    }
}

static Decor *GetDecor(DECORS type) 
{
    Decor *decor = malloc(sizeof(Decor));
    decor->type = type;

    decor->entity = CreateEntity();
    decor->entity->child = decor;

    decor->entity->spriteSheet = decorSpriteSheet;
    decor->entity->frameTexture = (Rectangle){TILE_SIZE * (decor->type % D_TILES_PER_ROW), TILE_SIZE * (decor->type / D_TILES_PER_ROW), TILE_SIZE, TILE_SIZE};

    decor->entity->Free = Free;
    decor->entity->SetDestination = SetDestination;

    return decor;
}

/************************
 * Core Funcs  
 ************************/

static void Free(Entity *decorEnt)
{
    int id = decorEnt->id;
    Decor *decor = (Decor *)decorEnt->child;

    free(decor);
    free(decorEnt);

    ENTITY_RECORD[id] = NULL;
}


/************************
 * Helper Funcs
 ************************/ 

static void SetDestination(Entity *decorEnt, float x, float y)
{
    Decor *decor = (Decor *)decorEnt->child;
    TextureData textureData = TEXTURES_DATA[TypeToIndex(decor->type)];

    decorEnt->destFrame.x = x;
    decorEnt->destFrame.y = y;

    decor->entity->collision.area.x = textureData.collision.area.x * SCALING_FACTOR + decor->entity->destFrame.x;
    decor->entity->collision.area.y = textureData.collision.area.y * SCALING_FACTOR + decor->entity->destFrame.y;
}

static DECORS IndexToType(int index) 
{
    switch (index)
    {
        case D_BOX_TOP_INDEX:
            return D_BOX_TOP;
        case D_BOX_BODY_INDEX:
            return D_BOX_BODY;
    }
}

static DECORS_INDEX TypeToIndex(DECORS type) 
{
    switch (type)
    {
        case D_BOX_TOP:
            return D_BOX_TOP_INDEX;
        case D_BOX_BODY:
            return D_BOX_BODY_INDEX;
    }
}