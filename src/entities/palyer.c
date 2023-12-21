#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "entity.h"
#include "utils.h"
#include "dev.h"

#define PlayerSpeed 120

static bool IsMoving();
static void Update(Entity *playerEnt);
static void Animate(Entity *playerEnt);

Player *GetSingletonPlayer()
{
    static Player *player = NULL;

    if (player == NULL)
    {
        player = malloc(sizeof(Player));
        player->entity = CreateEntity();

        Texture2D *spriteSheet = malloc(sizeof(Texture2D));
        *spriteSheet = LoadTexture(ASSET_PATH("players.png"));
        player->entity->spriteSheet = spriteSheet;
        player->entity->frameTexture = (Rectangle){0, 24, 16, 24};
        
        player->entity->destFrame = (Rectangle){0, 0, 16 * SCALING_FACTOR, 24 * SCALING_FACTOR};
        player->entity->origin = (Vector2){8, 12};
        
        player->entity->IsMoving = IsMoving;
        player->entity->Update = Update;  
        player->entity->Animate = Animate;
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
    
    angle = GetAngle(DEFAULT_INDEX);

    if (IsKeyDown(KEY_D)) angle                     = GetAngle(RIGHT_INDEX);
    if (IsKeyDown(KEY_S)) angle                     = GetAngle(DOWN_INDEX);
    if (IsKeyDown(KEY_A)) angle                     = GetAngle(LEFT_INDEX);
    if (IsKeyDown(KEY_W)) angle                     = GetAngle(UP_INDEX);
    if (IsKeyDown(KEY_S) && IsKeyDown(KEY_D)) angle = GetAngle(DOWN_RIGHT_INDEX);
    if (IsKeyDown(KEY_S) && IsKeyDown(KEY_A)) angle = GetAngle(DOWN_LEFT_INDEX);
    if (IsKeyDown(KEY_W) && IsKeyDown(KEY_A)) angle = GetAngle(UP_LEFT_INDEX);
    if (IsKeyDown(KEY_W) && IsKeyDown(KEY_D)) angle = GetAngle(UP_RIGHT_INDEX);

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
    if (angle == GetAngle(UP_INDEX))           index = UP_INDEX;
    if (angle == GetAngle(RIGHT_INDEX))        index = RIGHT_INDEX;
    if (angle == GetAngle(DOWN_INDEX))         index = DOWN_INDEX;
    if (angle == GetAngle(LEFT_INDEX))         index = LEFT_INDEX;
    if (angle == GetAngle(UP_RIGHT_INDEX))     index = UP_RIGHT_INDEX;
    if (angle == GetAngle(DOWN_RIGHT_INDEX))   index = DOWN_RIGHT_INDEX;
    if (angle == GetAngle(DOWN_LEFT_INDEX))    index = DOWN_LEFT_INDEX;
    if (angle == GetAngle(UP_LEFT_INDEX))      index = UP_LEFT_INDEX;

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

    UnloadTexture(*(playerEnt->spriteSheet));

    free(playerEnt);
    free(player);

    ENTITY_RECORD[id] = NULL;
    ENTITY_COUNT--;
    
}

/************************
 *                      *
 * Helper Funcs         *
 *                      *
 ************************/ 