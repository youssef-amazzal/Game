#include "utils.h"
#include "raylib.h"
#include <stdio.h>

float GetAngle(DIRECTION_INDEX direction) {
    switch (direction) {
        case RIGHT_INDEX:
            return 0;

        case DOWN_RIGHT_INDEX:
            return PI / 4;

        case DOWN_INDEX:
            return PI / 2;
        
        case DOWN_LEFT_INDEX:
            return 3 * PI / 4;
        
        case LEFT_INDEX:
            return PI;
        
        case UP_LEFT_INDEX:
            return 5 * PI / 4;
        
        case UP_INDEX:
            return 3 * PI / 2;
        
        case UP_RIGHT_INDEX:
            return 7 * PI / 4;
    };
}

