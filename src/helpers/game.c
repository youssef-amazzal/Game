#include "game.h"
#include "raylib.h"
#include "entity.h"
#include "math.h"
#include <stdlib.h>


/************************
 * Game Cycle Funcs
 ************************/
void SartGameCycle() {
    for (int i = 0; i < LAST_ID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->Update(ENTITY_RECORD[i]);
            ENTITY_RECORD[i]->Animate(ENTITY_RECORD[i]);
        }
    }
    
    SortRenderOrder();
    DetectHitBoxs();

    for (int i = 0; i < LAST_ID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->Render(ENTITY_RECORD[i]);
            ENTITY_RECORD[i]->React(ENTITY_RECORD[i]);
        }
    }
    
}

void StopGameCycle()
{
    for (int i = 0; i < LAST_ID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->Free(ENTITY_RECORD[i]);
        }
    }
}

/************************
 * Movement Funcs
 ************************/

char *DirectionToString(DIRECTIONS direction) {
    switch (direction) {
        case RIGHT:
            return "RIGHT";

        case DOWN_RIGHT:
            return "DOWN_RIGHT";

        case DOWN:
            return "DOWN";
        
        case DOWN_LEFT:
            return "DOWN_LEFT";
        
        case LEFT:
            return "LEFT";
        
        case UP_LEFT:
            return "UP_LEFT";
        
        case UP:
            return "UP";
        
        case UP_RIGHT:
            return "UP_RIGHT";
    };
}
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
DIRECTIONS AngleToDirection(float angle) {
    if (angle >= 0 && angle < PI / 8) {
        return RIGHT;
    }

    if (angle >= PI / 8 && angle < 3 * PI / 8) {
        return DOWN_RIGHT;
    }

    if (angle >= 3 * PI / 8 && angle < 5 * PI / 8) {
        return DOWN;
    }

    if (angle >= 5 * PI / 8 && angle < 7 * PI / 8) {
        return DOWN_LEFT;
    }

    if (angle >= 7 * PI / 8 && angle < 9 * PI / 8) {
        return LEFT;
    }

    if (angle >= 9 * PI / 8 && angle < 11 * PI / 8) {
        return UP_LEFT;
    }

    if (angle >= 11 * PI / 8 && angle < 13 * PI / 8) {
        return UP;
    }

    if (angle >= 13 * PI / 8 && angle < 15 * PI / 8) {
        return UP_RIGHT;
    }
}

Vector2 CalculateDestination(Vector2 position, Vector2 velocity, float angle) {
    float x = position.x + velocity.x * cos(angle) * GetFrameTime();
    float y = position.y + velocity.y * sin(angle) * GetFrameTime();

    return (Vector2) { x, y };
}

/************************
 * Render Funcs
 ************************/

void SortRenderOrder()
{
    for (int i = 0; i < LAST_ID; i++)
    {
        for (int j = 0; j < LAST_ID - i - 1; j++)
        {
            int z = ENTITY_RECORD[j]->zIndex;
            int y = ENTITY_RECORD[j]->destFrame.y;
            if (z > ENTITY_RECORD[j + 1]->zIndex || (z == ENTITY_RECORD[j + 1]->zIndex && y > ENTITY_RECORD[j + 1]->destFrame.y))
            {
                Entity *temp = ENTITY_RECORD[j];
                ENTITY_RECORD[j] = ENTITY_RECORD[j + 1];
                ENTITY_RECORD[j + 1] = temp;

                ENTITY_RECORD[j]->id = j;
                ENTITY_RECORD[j + 1]->id = j + 1;
            }
        }
    }
}

/************************
 * HitBox Funcs
 ************************/
Collision COLLISION_RECORD[MAX_ENTITIES][MAX_ENTITIES];

void DetectHitBoxs()
{
   for (int i = 0; i < LAST_ID; i++)
    {
        for (int j = i + 1; j < LAST_ID; j++)
        {
            Collision *collision = GetCollision(i, j);
            collision->isColliding = false;

            if (ENTITY_RECORD[i] != NULL && ENTITY_RECORD[j] != NULL)
            {
                if (CheckCollisionRecs(ENTITY_RECORD[i]->hitBox.area, ENTITY_RECORD[j]->hitBox.area))
                {
                    collision->area = GetCollisionRec(ENTITY_RECORD[i]->hitBox.area, ENTITY_RECORD[j]->hitBox.area);
                    collision->isColliding = true;
                }
            }
        }
    }
}
Collision *GetCollision(int id1, int id2)
{
    int minId = id1 < id2 ? id1 : id2;
    int maxId = id1 > id2 ? id1 : id2;

    return &COLLISION_RECORD[minId][maxId];
}