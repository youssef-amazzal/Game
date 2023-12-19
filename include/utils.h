#pragma once

enum DIRECTION_INDEX
{    
    UP_INDEX          = 0,
    UP_RIGHT_INDEX    = 1,
    RIGHT_INDEX       = 2,
    DOWN_RIGHT_INDEX  = 3,
    DOWN_INDEX        = 4,
    DOWN_LEFT_INDEX   = 5,
    LEFT_INDEX        = 6,
    UP_LEFT_INDEX     = 7,

    DEFAULT_INDEX     = DOWN_INDEX
};

float GetAngle(int direction);