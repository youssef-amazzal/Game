#include "entity.h"
#include "assets.h"
#include <stdlib.h>
#include <stdio.h>

Entity *ENTITY_RECORD[MAX_ENTITIES];
int LAST_ID = 0;

static void Update(Entity *e);
static void Animate(Entity *e);
static void Render(Entity *e);
static void React(Entity *e);
static void Free(Entity *e);

static bool IsMoving(Entity *e);
static void SetDestination(Entity *e, float x, float y);

Entity *CreateEntity() 
{
    Entity *e = malloc(sizeof(Entity));
    e->id = LAST_ID++;
    e->child = NULL;
    e->collision.owner = e->id;
    
    e->spriteSheet = NULL;
    e->frameTexture = (Rectangle){0, 0, TILE_SIZE, TILE_SIZE};

    e->zIndex = 0;
    
    e->destFrame = (Rectangle){0, 0, TILE_SIZE * SCALING_FACTOR, TILE_SIZE * SCALING_FACTOR};
    e->origin = (Vector2){0, 0};
    e->velocity = (Vector2){0, 0};
    e->angle = 0;

    e->Update = Update;
    e->Animate = Animate;
    e->Render = Render;
    e->React = React;
    e->Free = Free;

    e->IsMoving = IsMoving;
    e->SetDestination = SetDestination;
    
    ENTITY_RECORD[e->id] = e;
    return e;
}

/************************
 * Core Funcs
 ************************/ 

static void Update(Entity *e) {}

static void Animate(Entity *e){}

static void Render(Entity *e)
{
    static bool showcollision = false;
    DrawTexturePro(*(e->spriteSheet), e->frameTexture, e->destFrame, e->origin, 0, WHITE);
    if (showcollision) {
        DrawRectangleLinesEx(e->collision.area, 1, e->collision.color);
        for (int i = e->id + 1; i < LAST_ID; i++) {
            if (ENTITY_RECORD[i] != NULL) {
                if (INTERSECTION_RECORD[e->id][i].isColliding) {
                    DrawRectangleRec(INTERSECTION_RECORD[e->id][i].area, (Color){255, 180, 0, 100});
                }
            }
        }
    }
    if (IsKeyDown(KEY_SPACE)) showcollision = true; else showcollision = false;
}

static void React(Entity *e){
    if (e->isReactive) {
        
    }
}

static void Free(Entity *e)
{
    int id = e->id;
    
    free(e);
    
    ENTITY_RECORD[id] = NULL;
}

/************************
 * Helper Funcs
 ************************/

static bool IsMoving(Entity *e)
{
    return e->velocity.x != 0 || e->velocity.y != 0;
}

static void SetDestination(Entity *e, float x, float y) {}


