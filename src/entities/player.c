#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "entity.h"
#include "game.h"
#include "assets.h"

#define PlayerSpeed FRAME_RATE

int PLAYER_ID;

static bool IsMoving();
static void Update(Entity *e);
static void Animate(Entity *e);
static void React(Entity *e);
static void Free(Entity *e);

static void SetDestination(Entity *e, float x, float y);
static void handleKeyboardInputs(Entity *e);

Player *GetSingletonPlayer()
{
    Player *player;

    player = malloc(sizeof(Player));
    player->entity = CreateEntity();
    player->entity->child = player;

    player->entity->spriteSheet = playerSpriteSheet;
    player->entity->frameTexture = (Rectangle){0, 24, 16, 24};
    
    player->entity->destFrame = (Rectangle){16 * 3, 16 * 3, 16 * SCALING_FACTOR, 24 * SCALING_FACTOR};

    player->entity->hitBox.area = (Rectangle){16 * 3, 16 * 3, 16 * SCALING_FACTOR * 0.9, 24 * SCALING_FACTOR * 0.5};
    player->entity->hitBox.color = LIME;
    player->entity->hitBox.owner = player->entity->id;

    player->entity->hitBox.canBeBlocked = true;
    player->entity->hitBox.canPush = true;
    player->entity->hitBox.canCollect = true;
    player->entity->hitBox.canBeDestroyed = true;
    player->entity->hitBox.canBeTeleported = true;

    player->entity->Update = Update;  
    player->entity->Animate = Animate;
    player->entity->SetDestination = SetDestination;

    PLAYER_ID = player->entity->id;

    return player;
}

/************************
 * Core Funcs  
 ************************/
static void Update(Entity *e) {
    e->velocity = (Vector2){0, 0};
    handleKeyboardInputs(e);
    Vector2 dest = CalculateDestination((Vector2) {e->destFrame.x, e->destFrame.y}, e->velocity, e->angle);
    SetDestination(e, dest.x, dest.y);
}

static void Animate(Entity *e) {
    float angle = e->angle;
    int playerActualHeight = e->frameTexture.height;
    int playerActualWidth = e->frameTexture.width;
    
    static int animationCycle = 0, counter = 0;
    animationCycle = e->IsMoving(e) ? animationCycle + 1 : 2;
    

    int standPositionIndex = playerActualHeight * 2;

    int index;
    if (angle == DirectionToAngle(UP))           index = UP;
    if (angle == DirectionToAngle(RIGHT))        index = RIGHT;
    if (angle == DirectionToAngle(DOWN))         index = DOWN;
    if (angle == DirectionToAngle(LEFT))         index = LEFT;
    // if (angle == DirectionToAngle(UP_RIGHT))     index = UP_RIGHT;
    // if (angle == DirectionToAngle(DOWN_RIGHT))   index = DOWN_RIGHT;
    // if (angle == DirectionToAngle(DOWN_LEFT))    index = DOWN_LEFT;
    // if (angle == DirectionToAngle(UP_LEFT))      index = UP_LEFT;
    
    e->frameTexture.x = playerActualWidth * index;
    e->frameTexture.y = e->IsMoving(e) ? playerActualHeight * (animationCycle / 15 % 3 + 1) : standPositionIndex;
}

static void Free(Entity *e)
{
    int id = e->id;
    Player *player = (Player *)e->child;

    free(e);
    free(player);

    ENTITY_RECORD[id] = NULL;
    
}

/************************
 * Helper Funcs
 ************************/ 

static void SetDestination(Entity *e, float x, float y)
{
    Player *player = (Player *)e->child;

    e->destFrame.x = x;
    e->destFrame.y = y;

    
    player->entity->hitBox.area.x = e->destFrame.x + e->destFrame.width / 2 - player->entity->hitBox.area.width / 2;
    player->entity->hitBox.area.y = e->destFrame.y + e->destFrame.height / 2 - player->entity->hitBox.area.height / 2;
}

static void handleKeyboardInputs(Entity *e)
{
    float angle = DirectionToAngle(AngleToDirection(e->angle));

    static int lastPressedKey = 0;
    bool lockKey = false;
    if (IsKeyDown(lastPressedKey)) lockKey = true;

    if (!lockKey && IsKeyDown(KEY_D) || lockKey && lastPressedKey == KEY_D) {
        angle = DirectionToAngle(RIGHT);
        e->velocity.x = PlayerSpeed;
        lastPressedKey = KEY_D;
    }
    
    if (!lockKey && IsKeyDown(KEY_S) || lockKey && lastPressedKey == KEY_S) {
        angle = DirectionToAngle(DOWN);
        e->velocity.y = PlayerSpeed;
        lastPressedKey = KEY_S;
    }

    if (!lockKey && IsKeyDown(KEY_A) || lockKey && lastPressedKey == KEY_A) {
        angle = DirectionToAngle(LEFT);
        e->velocity.x = PlayerSpeed;
        lastPressedKey = KEY_A;
    }

    if (!lockKey && IsKeyDown(KEY_W) || lockKey && lastPressedKey == KEY_W) {
        angle = DirectionToAngle(UP);
        e->velocity.y = PlayerSpeed;
        lastPressedKey = KEY_W;
    }
    // if (IsKeyDown(KEY_S) && IsKeyDown(KEY_D)) angle = DirectionToAngle(DOWN_RIGHT);
    // if (IsKeyDown(KEY_S) && IsKeyDown(KEY_A)) angle = DirectionToAngle(DOWN_LEFT);
    // if (IsKeyDown(KEY_W) && IsKeyDown(KEY_A)) angle = DirectionToAngle(UP_LEFT);
    // if (IsKeyDown(KEY_W) && IsKeyDown(KEY_D)) angle = DirectionToAngle(UP_RIGHT);

    e->angle = angle;
}

// static bool IsMoving()
// {
//     bool moving = false;

//     if (IsKeyDown(KEY_W)) moving = true;
//     if (!moving && IsKeyDown(KEY_D)) moving = true;
//     if (!moving && IsKeyDown(KEY_S)) moving = true;
//     if (!moving && IsKeyDown(KEY_A)) moving = true;

//     return moving;
// }