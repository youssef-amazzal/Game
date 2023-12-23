#include "game.h"
#include "raylib.h"
#include "entity.h"
#include "math.h"
#include <stdlib.h>


/************************
 * Entities management funcs
 ************************/
void StartAll() {
    for (int i = 0; i < LAST_ID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->Update(ENTITY_RECORD[i]);
            ENTITY_RECORD[i]->Animate(ENTITY_RECORD[i]);
        }
    }
    
    SortRenderOrder();
    InitCollisions();

    for (int i = 0; i < LAST_ID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->React(ENTITY_RECORD[i]);
            ENTITY_RECORD[i]->Render(ENTITY_RECORD[i]);
        }
    }
    
}

void FreeAll()
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
 * Collision Funcs
 ************************/
Intersection INTERSECTION_RECORD[MAX_ENTITIES][MAX_ENTITIES];

void InitCollisions()
{
   for (int i = 0; i < LAST_ID; i++)
    {
        for (int j = i + 1; j < LAST_ID; j++)
        {
            INTERSECTION_RECORD[i][j].isColliding = false;

            if (ENTITY_RECORD[i] != NULL && ENTITY_RECORD[j] != NULL)
            {
                if (CheckCollisionRecs(ENTITY_RECORD[i]->collision.area, ENTITY_RECORD[j]->collision.area))
                {
                    Rectangle intersection = GetCollisionRec(ENTITY_RECORD[i]->collision.area, ENTITY_RECORD[j]->collision.area);
                    INTERSECTION_RECORD[i][j].area = intersection;
                    INTERSECTION_RECORD[i][j].isColliding = true;
                }
            }
        }
    }
}

// TODO: have a doubt whether I need to find a way to call those reactions only 
//       for the first participant of the collision and skip the second one

void PushReaction(Entity *pusher, Entity *pushed, void (*setDestination)(Entity *pushed, float x, float y)) {
    if (pushed->collision.canBePushed) {
        Vector2 dest = CalculateDestination((Vector2) {pushed->destFrame.x, pushed->destFrame.y}, pusher->velocity, pusher->angle);
        setDestination(pushed, dest.x, dest.y);
    }
}

void BlockReaction(Entity *blocker, Entity *blocked, void (*setDestination)(Entity *blocked, float x, float y)) {
    if (blocked->collision.canBeBlocked) {
        Vector2 dest = CalculateDestination((Vector2) {blocked->destFrame.x, blocked->destFrame.y}, (Vector2) {0, 0}, blocker->angle);
        setDestination(blocked, dest.x, dest.y);
    }
}
