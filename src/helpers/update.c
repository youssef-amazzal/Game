#include "update.h"
#include "raylib.h"

float DirectionToAngle(DIRECTIONS direction) {
    switch (direction) {
        case RIGHT:
            return 0;

        case DOWN_RIGHT:
            return PI / 4;

        case DOWN:
            return PI / 2;
        
        case DOWN_LEFT:
            return 3 * PI / 4;
        
        case LEFT:
            return PI;
        
        case UP_LEFT:
            return 5 * PI / 4;
        
        case UP:
            return 3 * PI / 2;
        
        case UP_RIGHT:
            return 7 * PI / 4;
    };
}
