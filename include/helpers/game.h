#pragma once
#include "raylib.h"

#define FRAME_RATE 120

/************************
 * Entities management
 ************************/
#define MAX_ENTITIES 400
extern int PLAYER_ID;

void SartGameCycle();
void StopGameCycle();

/************************
 * Movement
 ************************/
typedef enum DIRECTIONS
{    
    UP         = 0,
    UP_RIGHT   = 1,
    RIGHT      = 2,
    DOWN_RIGHT = 3,
    DOWN       = 4,
    DOWN_LEFT  = 5,
    LEFT       = 6,
    UP_LEFT    = 7,

    DEFAULT    = DOWN
} DIRECTIONS ;

char *DirectionToString(DIRECTIONS direction);
float DirectionToAngle(DIRECTIONS direction);
DIRECTIONS AngleToDirection(float angle);
Vector2 CalculateDestination(Vector2 position, Vector2 velocity, float angle);

/************************
 * Render
 * **********************/
void SortRenderOrder();

/************************
 * HitBox
 ************************/

typedef enum HITBOX_TYPE {
    HITBOX_RECTANGLE,
    HITBOX_CIRCLE,
} HITBOX_TYPE;

typedef struct HitBox {
    int owner;
    HITBOX_TYPE type;
    Rectangle area;
    Color color;

    bool isDisabled;

    bool canBlock;
    bool canBeBlocked;

    bool canPush;
    bool canBePushed;

    bool canDestroy;
    bool canBeDestroyed;

    bool canCollect;
    bool canBeCollected;

    bool canTeleport;
    bool canBeTeleported;

    bool canBounce;
    bool canBeBounced;

} HitBox;

typedef struct Collision {
    Rectangle area;
    bool isColliding;
} Collision;

extern Collision COLLISION_RECORD[MAX_ENTITIES][MAX_ENTITIES];
Collision *GetCollision(int id1, int id2);
void DetectHitBoxs();


