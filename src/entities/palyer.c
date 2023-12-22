#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "entity.h"
#include "update.h"
#include "assets.h"

#define PlayerSpeed 120

static bool IsMoving();
static void Update(Entity *playerEnt);
static void Animate(Entity *playerEnt);

Player *GetSingletonPlayer()
{
    static bool initialized = false;
    static Player *player;

    if (!initialized)
    {
        player = malloc(sizeof(Player));
        player->entity = CreateEntity();
        player->entity->child = player;

        player->entity->spriteSheet = playerSpriteSheet;
        player->entity->frameTexture = (Rectangle){0, 24, 16, 24};
        
        player->entity->destFrame = (Rectangle){0, 0, 16 * SCALING_FACTOR, 24 * SCALING_FACTOR};
        player->entity->origin = (Vector2){8, 12};
        
        player->entity->IsMoving = IsMoving;
        player->entity->Update = Update;  
        player->entity->Animate = Animate;

        initialized = true;
    }

    return player;
}

/************************
 *                      *
 * Core Funcs           *
 *                      *
 ************************/
static void Update(Entity *playerEnt) {
    float xAxis = PlayerSpeed, yAxis = PlayerSpeed, angle;
    
    angle = DirectionToAngle(DEFAULT);

    if (IsKeyDown(KEY_D)) angle                     = DirectionToAngle(RIGHT);
    if (IsKeyDown(KEY_S)) angle                     = DirectionToAngle(DOWN);
    if (IsKeyDown(KEY_A)) angle                     = DirectionToAngle(LEFT);
    if (IsKeyDown(KEY_W)) angle                     = DirectionToAngle(UP);
    if (IsKeyDown(KEY_S) && IsKeyDown(KEY_D)) angle = DirectionToAngle(DOWN_RIGHT);
    if (IsKeyDown(KEY_S) && IsKeyDown(KEY_A)) angle = DirectionToAngle(DOWN_LEFT);
    if (IsKeyDown(KEY_W) && IsKeyDown(KEY_A)) angle = DirectionToAngle(UP_LEFT);
    if (IsKeyDown(KEY_W) && IsKeyDown(KEY_D)) angle = DirectionToAngle(UP_RIGHT);

    playerEnt->velocity.x = xAxis;
    playerEnt->velocity.y = yAxis;
    playerEnt->angle = angle;

    if (playerEnt->IsMoving())
    {
        playerEnt->destFrame.x += playerEnt->velocity.x * cos(playerEnt->angle) * GetFrameTime();
        playerEnt->destFrame.y += playerEnt->velocity.y * sin(playerEnt->angle) * GetFrameTime();
    }
}

static void Animate(Entity *playerEnt) {
    float angle = playerEnt->angle;
    int playerActualHeight = playerEnt->frameTexture.height;
    int playerActualWidth = playerEnt->frameTexture.width;
    
    static int animationCycle = 0, counter = 0;
    animationCycle = playerEnt->IsMoving() ? animationCycle + 1 : 2;

    int standPositionIndex = playerActualHeight * 2;

    int index;
    if (angle == DirectionToAngle(UP))           index = UP;
    if (angle == DirectionToAngle(RIGHT))        index = RIGHT;
    if (angle == DirectionToAngle(DOWN))         index = DOWN;
    if (angle == DirectionToAngle(LEFT))         index = LEFT;
    if (angle == DirectionToAngle(UP_RIGHT))     index = UP_RIGHT;
    if (angle == DirectionToAngle(DOWN_RIGHT))   index = DOWN_RIGHT;
    if (angle == DirectionToAngle(DOWN_LEFT))    index = DOWN_LEFT;
    if (angle == DirectionToAngle(UP_LEFT))      index = UP_LEFT;

    playerEnt->frameTexture.x = playerActualWidth * index;
    playerEnt->frameTexture.y = playerEnt->IsMoving() ? playerActualHeight * (animationCycle / 15 % 3 + 1) : standPositionIndex;
}

static bool IsMoving()
{
    bool moving = false;

    if (IsKeyDown(KEY_W)) moving = true;
    if (!moving && IsKeyDown(KEY_D)) moving = true;
    if (!moving && IsKeyDown(KEY_S)) moving = true;
    if (!moving && IsKeyDown(KEY_A)) moving = true;

    return moving;
}

static void Free(Entity *playerEnt)
{
    int id = playerEnt->id;
    Player *player = (Player *)playerEnt->child;

    free(playerEnt);
    free(player);

    ENTITY_RECORD[id] = NULL;
    
}

/************************
 *                      *
 * Helper Funcs         *
 *                      *
 ************************/ 