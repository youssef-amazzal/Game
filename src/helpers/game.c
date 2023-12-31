#include "game.h"
#include "raylib.h"
#include "entity.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>


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
    // WORKAROUND: player colision should be handled first cause he gives his velocity 
    // to other entities but the entities with lower id reset it to 0 before being able to use it
    ENTITY_RECORD[PLAYER_ID]->React(ENTITY_RECORD[PLAYER_ID]);  

    for (int i = 0; i < LAST_ID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->React(ENTITY_RECORD[i]);
            ENTITY_RECORD[i]->Render(ENTITY_RECORD[i]);
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

        default:
            return 0;
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
                if (PLAYER_ID == ENTITY_RECORD[j]->id) PLAYER_ID = ENTITY_RECORD[j + 1]->id;
                else if (PLAYER_ID == ENTITY_RECORD[j + 1]->id) PLAYER_ID = ENTITY_RECORD[j]->id;
                
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

DIRECTIONS DetectCollisionDirection(int colliderId, int collidedId)
{
    Entity *collider = ENTITY_RECORD[colliderId];
    Entity *collided = ENTITY_RECORD[collidedId];

    if (!CheckCollision(collider->id, collided->id)) return -1;

    // using the destination frame to calculate the collision area save me the trouble
    // when the hitbox is a circle
    Rectangle collisionArea = GetCollisionRec(collider->destFrame, collided->destFrame);
    
    DIRECTIONS direction = -1;

    if (collisionArea.width > collisionArea.height) {
        if (collider->destFrame.y > collided->destFrame.y) {
            direction = DOWN;
        } else {
            direction = UP;
        }
    }

    if (collisionArea.width < collisionArea.height) {
        if (collider->destFrame.x > collided->destFrame.x) {
            direction = RIGHT;
        } else {
            direction = LEFT;
        }
    }
    
    if (collisionArea.width == collisionArea.height) {
        if (collider->destFrame.x > collided->destFrame.x) {
            if (collider->destFrame.y > collided->destFrame.y) {
                direction = DOWN_RIGHT;
            } else {
                direction = UP_RIGHT;
            }
        } else {
            if (collider->destFrame.y > collided->destFrame.y) {
                direction = DOWN_LEFT;
            } else {
                direction = UP_LEFT;
            }
        }
    }

    return direction;
}

bool CheckCollision(int e1, int e2)
{
    Entity *entity1 = ENTITY_RECORD[e1];
    Entity *entity2 = ENTITY_RECORD[e2];
    
    if (entity1->hitBox.type == HITBOX_RECTANGLE && entity2->hitBox.type == HITBOX_RECTANGLE) return CheckCollisionRecs(entity1->hitBox.area, entity2->hitBox.area);
    if (entity1->hitBox.type == HITBOX_CIRCLE && entity2->hitBox.type == HITBOX_CIRCLE) return CheckCollisionCircles(entity1->hitBox.circle.center, entity1->hitBox.circle.radius, entity2->hitBox.circle.center, entity2->hitBox.circle.radius);
    if (entity1->hitBox.type == HITBOX_RECTANGLE && entity2->hitBox.type == HITBOX_CIRCLE) return CheckCollisionCircleRec(entity2->hitBox.circle.center, entity2->hitBox.circle.radius, entity1->hitBox.area);
    if (entity1->hitBox.type == HITBOX_CIRCLE && entity2->hitBox.type == HITBOX_RECTANGLE) return CheckCollisionCircleRec(entity1->hitBox.circle.center, entity1->hitBox.circle.radius, entity2->hitBox.area);
}

