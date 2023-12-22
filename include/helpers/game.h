#pragma once
#include "raylib.h"

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

/****************************************************/

typedef struct Collision {
    Rectangle area;
    Color color;

    bool isColliding;
    bool isDisabled;

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