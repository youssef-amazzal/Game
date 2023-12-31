#include "decor.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

const TextureData TEXTURES_DATA[] = {
    {
        .spriteSheet = GetDecorSpriteSheet,
        .tilesPerRow = D_TILES_PER_ROW,
        .tileTypeId = D_BOX_TOP, 
        .hitBox = {
            .area = {
                0,
                0,
                0,
                0,
            },
            .color = RED,
        },
        .zIndex = 1,
        .isExtension = true,
    },
    {
        .spriteSheet = GetDecorSpriteSheet,
        .tilesPerRow = D_TILES_PER_ROW,
        .tileTypeId = D_BOX_BODY ,
        .hitBox = {
            .area = {
                0,
                0,
                TILE_SIZE,
                TILE_SIZE,
            },
            .color = RED,
            .canBePushed = true,
            // .canPush = true,
            .canBounce = true,
            .canBeBlocked = true,
            .canBlock = true,
        },
        .isExtendable = true,
        .extensionPointsCount = 1,
        .extensionPoints = {
            {0, -1}
        },
            
    },
    {
        .spriteSheet = GetKeySpriteSheet,
        .tilesPerRow = K_TILES_PER_ROW,
        .tileTypeId = K_KEY,
        .hitBox = {
            .area = {
                0,
                TILE_SIZE / 4,
                TILE_SIZE,
                TILE_SIZE / 4,
            },
            .color = PINK,
            .canBeCollected = true,
        },
    }
};

static Decor *GetDecor(DECORS type);
static void Free(Entity *decorEnt);
static void SetDestination(Entity *decorEnt, float x, float y);
static DECORS IndexToType(int index);
static DECORS_INDEX TypeToIndex(DECORS type);

void InitDecors() 
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
                TextureData textureData = TEXTURES_DATA[TypeToIndex(layerArray[row][col])];
                if (textureData.isExtension) continue;

                Decor *decor = GetDecor(layerArray[row][col]);

                decor->entity->spriteSheet = textureData.spriteSheet();
                decor->entity->frameTexture = (Rectangle){TILE_SIZE * (decor->type % textureData.tilesPerRow), TILE_SIZE * (decor->type / textureData.tilesPerRow), TILE_SIZE, TILE_SIZE};

                decor->entity->hitBox.area.width = textureData.hitBox.area.width * SCALING_FACTOR;
                decor->entity->hitBox.area.height = textureData.hitBox.area.height * SCALING_FACTOR;
                decor->entity->hitBox.color = textureData.hitBox.color;

                decor->entity->hitBox.canBlock = textureData.hitBox.canBlock;
                decor->entity->hitBox.canBeBlocked = textureData.hitBox.canBeBlocked;

                decor->entity->hitBox.canPush = textureData.hitBox.canPush;
                decor->entity->hitBox.canBePushed = textureData.hitBox.canBePushed;

                decor->entity->hitBox.canDestroy = textureData.hitBox.canDestroy;
                decor->entity->hitBox.canBeDestroyed = textureData.hitBox.canBeDestroyed;

                decor->entity->hitBox.canCollect = textureData.hitBox.canCollect;
                decor->entity->hitBox.canBeCollected = textureData.hitBox.canBeCollected;

                decor->entity->hitBox.canBounce = textureData.hitBox.canBounce;
                decor->entity->hitBox.canBeBounced = textureData.hitBox.canBeBounced;

                decor->entity->zIndex = textureData.zIndex;

                SetDestination(decor->entity, col * TILE_SIZE * SCALING_FACTOR, row * TILE_SIZE * SCALING_FACTOR);

                decor->entity->extensionCount = textureData.extensionPointsCount;
                
                if (textureData.isExtendable) 
                {
                    decor->entity->extensions = malloc(sizeof(Entity *) * textureData.extensionPointsCount);

                    for (int i = 0; i < textureData.extensionPointsCount; i++)
                    {
                        int x = col + textureData.extensionPoints[i].x;
                        int y = row + textureData.extensionPoints[i].y;

                        Decor *extension = GetDecor(IndexToType(layerArray[y][x]));
                        textureData = TEXTURES_DATA[TypeToIndex(layerArray[y][x])];

                        extension->entity->spriteSheet = textureData.spriteSheet();
                        extension->entity->frameTexture = (Rectangle){TILE_SIZE * (extension->type % textureData.tilesPerRow), TILE_SIZE * (extension->type / textureData.tilesPerRow), TILE_SIZE, TILE_SIZE};

                        extension->entity->hitBox.area.width = textureData.hitBox.area.width * SCALING_FACTOR;
                        extension->entity->hitBox.area.height = textureData.hitBox.area.height * SCALING_FACTOR;
                        extension->entity->hitBox.color = textureData.hitBox.color;

                        extension->entity->hitBox.canBlock = textureData.hitBox.canBlock;
                        extension->entity->hitBox.canBePushed = textureData.hitBox.canBePushed;
                        extension->entity->hitBox.canBeDestroyed = textureData.hitBox.canBeDestroyed;
                        extension->entity->hitBox.canDestroy = textureData.hitBox.canDestroy;
                        extension->entity->hitBox.canBeCollected = textureData.hitBox.canBeCollected;
                        extension->entity->hitBox.canBounce = textureData.hitBox.canBounce;

                        extension->entity->zIndex = textureData.zIndex;

                        SetDestination(extension->entity, x * TILE_SIZE * SCALING_FACTOR, y * TILE_SIZE * SCALING_FACTOR);

                        decor->entity->extensions[i] = extension->entity;
                    }
                }
            }
        }
    }
}

static Decor *GetDecor(DECORS type) 
{
    Decor *decor = malloc(sizeof(Decor));
    decor->type = type;

    if (decor->type == K_KEY) NBR_OF_KEYS++;

    decor->entity = CreateEntity();
    decor->entity->child = decor;

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

    float deltaX = x - decorEnt->destFrame.x, deltaY = y - decorEnt->destFrame.y;
    decorEnt->destFrame.x = x;
    decorEnt->destFrame.y = y;

    decor->entity->hitBox.area.x = textureData.hitBox.area.x * SCALING_FACTOR + decor->entity->destFrame.x;
    decor->entity->hitBox.area.y = textureData.hitBox.area.y * SCALING_FACTOR + decor->entity->destFrame.y;
    if (decor->entity->extensionCount) 
    {
        for (int i = 0; i < decor->entity->extensionCount; i++)
        {
            Entity *extension = decor->entity->extensions[i];
            
            float oldX = extension->destFrame.x;
            float oldY = extension->destFrame.y;

            float newX = oldX + deltaX;
            float newY = oldY + deltaY;


            SetDestination(extension, newX, newY);
        }
    }
}

static DECORS IndexToType(int index) 
{
    switch (index)
    {
        case D_BOX_TOP_INDEX:
            return D_BOX_TOP;
        case D_BOX_BODY_INDEX:
            return D_BOX_BODY;
        case K_KEY_INDEX:
            return K_KEY;
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
        case K_KEY:
            return K_KEY_INDEX;
    }
}