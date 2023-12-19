#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "entity.h"
#include "utils.h"


static int isMoving();
static void UpdatePlayer(Entity *playerEnt);
static void TrackPlayerMovement(Entity *playerEnt);
static void AnimatePlayer(Entity *playerEnt);
static void UpdatePlayerPosition(Entity *playerEnt);

Player *GetPlayer()
{
    static Player *player = NULL;

    if (player == NULL)
    {
        Texture2D *spriteSheet = malloc(sizeof(Texture2D));
        *spriteSheet = LoadTexture("sprite.png");
        Rectangle frameTexture = {0, 24, 16, 24};
        Rectangle destFrame = {0, 0, frameTexture.width * 3, frameTexture.height * 3};
        Vector2 origin = {frameTexture.width / 2, frameTexture.height / 2};
        
        player = malloc(sizeof(Player));
        player->entity = CreateEntity(spriteSheet, frameTexture, destFrame, origin, (Vector2){0,0}, isMoving);
        player->entity->update = UpdatePlayer;   
    }

    return player;
}

static int isMoving()
{
    int moving = 0;

    if (IsKeyDown(KEY_W))
        moving = 1;
    if (IsKeyDown(KEY_D))
        moving = 1;
    if (IsKeyDown(KEY_S))
        moving = 1;
    if (IsKeyDown(KEY_A))
        moving = 1;

    return moving;
}

static void TrackPlayerMovement(Entity *playerEnt)
{
    float xAxis = 120, yAxis = 120, angle;
    
    angle = GetAngle(DEFAULT_INDEX);

    if (IsKeyDown(KEY_D)) angle                     = GetAngle(RIGHT_INDEX);
    if (IsKeyDown(KEY_S)) angle                     = GetAngle(DOWN_INDEX);
    if (IsKeyDown(KEY_A)) angle                     = GetAngle(LEFT_INDEX);
    if (IsKeyDown(KEY_W)) angle                     = GetAngle(UP_INDEX);
    if (IsKeyDown(KEY_S) && IsKeyDown(KEY_D)) angle = GetAngle(DOWN_RIGHT_INDEX);
    if (IsKeyDown(KEY_S) && IsKeyDown(KEY_A)) angle = GetAngle(DOWN_LEFT_INDEX);
    if (IsKeyDown(KEY_W) && IsKeyDown(KEY_A)) angle = GetAngle(UP_LEFT_INDEX);
    if (IsKeyDown(KEY_W) && IsKeyDown(KEY_D)) angle = GetAngle(UP_RIGHT_INDEX);

    SetVelocity(playerEnt, (Vector2){xAxis, yAxis}, angle);
}

static void AnimatePlayer(Entity *playerEnt)
{
    float angle = playerEnt->angle;
    int playerActualHeight = playerEnt->frameTexture.height;
    int playerActualWidth = playerEnt->frameTexture.width;
    
    static int animationCycle = 0, counter = 0;
    animationCycle = playerEnt->isMoving() ? animationCycle + 1 : 2;

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
    playerEnt->frameTexture.y = playerEnt->isMoving() ? playerActualHeight * (animationCycle / 15 % 3 + 1) : standPositionIndex;
}

static void UpdatePlayerPosition(Entity *playerEnt)
{
    if (playerEnt->isMoving())
    {
        playerEnt->destFrame.x += playerEnt->velocity.x * cos(playerEnt->angle) * GetFrameTime();
        playerEnt->destFrame.y += playerEnt->velocity.y * sin(playerEnt->angle) * GetFrameTime();
    }
}

static void UpdatePlayer(Entity *playerEnt)
{
    TrackPlayerMovement(playerEnt);
    UpdatePlayerPosition(playerEnt);
    AnimatePlayer(playerEnt);
}