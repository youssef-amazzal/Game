#pragma once
#include "raylib.h"

#define FRAME_RATE 120

/************************
 * Entities management
 ************************/
#define MAX_ENTITIES 400
extern int PLAYER_ID;

void StartAll();
void FreeAll();

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

float DirectionToAngle(DIRECTIONS direction);
Vector2 CalculateDestination(Vector2 position, Vector2 velocity, float angle);

/************************
 * Render
 * **********************/
void SortRenderOrder();

/************************
 * Collision
 ************************/

typedef struct Collision {
    int owner;
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

} Collision;

typedef struct Intersection {
    Rectangle area;
    bool isColliding;
} Intersection;

extern Intersection INTERSECTION_RECORD[MAX_ENTITIES][MAX_ENTITIES];

void InitCollisions();


