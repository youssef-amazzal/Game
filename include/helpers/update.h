#pragma once

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