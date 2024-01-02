#include "ball.h"
#include "assets.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

#define BALL_SPEED FRAME_RATE * 3

static const TextureData TEXTURES_DATA[] = {
    {
        .tileTypeId = B_GRAY, 
        .hitBox = {
            .type = HITBOX_CIRCLE,
            .circle = {
                .center = {
                    TILE_SIZE / 2,
                    TILE_SIZE / 2
                },
                .radius = TILE_SIZE / 2,
            },
            .area = {
                0,
                0,
                0, 
                0
            },
            .color = PURPLE,
            .canBeBounced = true,
            .canBounce = true,
            .canBeBlocked = true,
        },
    },
    {
        .tileTypeId = B_BROWN, 
        .hitBox = {
            .type = HITBOX_CIRCLE,
            .circle = {
                .center = {
                    TILE_SIZE / 2,
                    TILE_SIZE / 2
                },
                .radius = TILE_SIZE / 2,
            },
            .area = {
                0,
                0,
                0, 
                0
            },
            .color = PURPLE,
            .canBeBounced = true,
            .canBounce = true,
            .canBeBlocked = true,
        },
    },
};

static Ball *GetBall(BALLS type);
static void Update(Entity *ballEnt);
static void Free(Entity *ballEnt);
static void SetDestination(Entity *ballEnt, float x, float y);
static BALLS IndexToType(int index);
static BALLS_INDEX TypeToIndex(BALLS type);

void InitBalls()
{
    static bool initialized = false;
    if (!initialized) 
    {
        int width = CURRENT_LEVEL->width;
        int height = CURRENT_LEVEL->height;

        int **layerArray = CURRENT_LEVEL->layers[LAYER_BALL];

        for (int row = 0; row < height; row++) 
        {
            for (int col = 0; col < width; col++)
            {
                if (layerArray[row][col] != -1) 
                {
                    TextureData textureData = TEXTURES_DATA[TypeToIndex(layerArray[row][col])];
                    if (textureData.isExtension) continue;
                    
                    Ball *ball = GetBall(layerArray[row][col]);
                    
                    ball->entity->hitBox.type = textureData.hitBox.type;
                    ball->entity->hitBox.circle.radius = textureData.hitBox.circle.radius * SCALING_FACTOR;
                    ball->entity->hitBox.color = textureData.hitBox.color;

                    ball->entity->hitBox.canBeBounced = textureData.hitBox.canBeBounced;
                    ball->entity->hitBox.canBounce = textureData.hitBox.canBounce;

                    ball->entity->hitBox.canBeBlocked = textureData.hitBox.canBeBlocked;

                    ball->entity->zIndex = textureData.zIndex;

                    SetDestination(ball->entity, col * TILE_SIZE * SCALING_FACTOR, row * TILE_SIZE * SCALING_FACTOR);

                    ball->entity->extensionCount = textureData.extensionPointsCount;
                    if (textureData.isExtendable) 
                    {
                        ball->entity->extensions = malloc(sizeof(Entity *) * textureData.extensionPointsCount);

                        for (int i = 0; i < textureData.extensionPointsCount; i++)
                        {
                            int x = col + textureData.extensionPoints[i].x;
                            int y = row + textureData.extensionPoints[i].y;

                            Ball *extension = GetBall(IndexToType(layerArray[y][x]));
                            textureData = TEXTURES_DATA[TypeToIndex(layerArray[y][x])];

                            extension->entity->hitBox.circle.radius = textureData.hitBox.circle.radius * SCALING_FACTOR;
                            extension->entity->hitBox.color = textureData.hitBox.color;

                            extension->entity->hitBox.canBeBounced = textureData.hitBox.canBeBounced;

                            extension->entity->zIndex = textureData.zIndex;

                            SetDestination(extension->entity, x * TILE_SIZE * SCALING_FACTOR, y * TILE_SIZE * SCALING_FACTOR);

                            ball->entity->extensions[i] = extension->entity;
                        }
                    }
                }
            }
        }
        
        initialized = true;
    }
}

static Ball *GetBall(BALLS type) 
{
    Ball *ball = malloc(sizeof(Ball));
    ball->type = type;

    ball->entity = CreateEntity();
    ball->entity->child = ball;

    ball->entity->spriteSheet = ballSpriteSheet;
    ball->entity->frameTexture = (Rectangle){TILE_SIZE * 2 * (ball->type % B_TILES_PER_ROW), TILE_SIZE * 2 * (ball->type / B_TILES_PER_ROW), TILE_SIZE * 2, TILE_SIZE * 2};

    ball->entity->velocity = (Vector2){BALL_SPEED, BALL_SPEED};
    ball->entity->angle = (float)rand() / RAND_MAX * (PI / 2);

    ball->entity->Update = Update;
    ball->entity->Free = Free;
    ball->entity->SetDestination = SetDestination;

    return ball;
}

/************************
 * Core Funcs  
 ************************/

static void Update(Entity *ballEnt) 
{
    Vector2 dest = CalculateDestination((Vector2) {ballEnt->destFrame.x, ballEnt->destFrame.y}, ballEnt->velocity, ballEnt->angle);
    SetDestination(ballEnt, dest.x, dest.y);
}

static void Free(Entity *ballEnt)
{
    int id = ballEnt->id;
    Ball *ball = (Ball *)ballEnt->child;

    free(ball);
    free(ballEnt);

    ENTITY_RECORD[id] = NULL;
}


/************************
 * Helper Funcs
 ************************/ 

static void SetDestination(Entity *ballEnt, float x, float y)
{
    Ball *ball = (Ball *)ballEnt->child;
    TextureData textureData = TEXTURES_DATA[TypeToIndex(ball->type)];

    float deltaX = x - ballEnt->destFrame.x, deltaY = y - ballEnt->destFrame.y;
    ballEnt->destFrame.x = x;
    ballEnt->destFrame.y = y;

    ball->entity->hitBox.circle.center.x = textureData.hitBox.circle.center.x * SCALING_FACTOR + ballEnt->destFrame.x;
    ball->entity->hitBox.circle.center.y = textureData.hitBox.circle.center.y * SCALING_FACTOR + ballEnt->destFrame.y;

    if (ball->entity->extensionCount) 
    {
        for (int i = 0; i < ball->entity->extensionCount; i++)
        {
            Entity *extension = ball->entity->extensions[i];
            
            float oldX = extension->destFrame.x;
            float oldY = extension->destFrame.y;

            float newX = oldX + deltaX;
            float newY = oldY + deltaY;

            SetDestination(extension, newX, newY);
        }
    }
}

static BALLS IndexToType(int index) 
{
    switch (index)
    {
        case B_BROWN_INDEX:
            return B_BROWN;
        case B_GRAY_INDEX:
            return B_GRAY;
    }
}

static BALLS_INDEX TypeToIndex(BALLS type) 
{
    switch (type)
    {
        case B_BROWN:
            return B_BROWN_INDEX;
        case B_GRAY:
            return B_GRAY_INDEX;
    }
}