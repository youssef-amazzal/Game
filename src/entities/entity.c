#include "entity.h"
#include "assets.h"
#include <stdlib.h>
#include <stdio.h>

Entity *ENTITY_RECORD[MAX_ENTITIES];
static int lastEntityID = 0;

static void Update(Entity *e);
static void Animate(Entity *e);
static void Render(Entity *e);
static bool IsMoving(Entity *e);
static void Free(Entity *e);
static void SortRenderOrder();

Entity *CreateEntity() 
{
    Entity *e = malloc(sizeof(Entity));
    e->id = lastEntityID++;
    e->child = NULL;
    
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
    e->IsMoving = IsMoving;
    
    ENTITY_RECORD[e->id] = e;
    return e;
}

/************************
 *                      *
 * Core Funcs           *
 *                      *
 ************************/ 

// since not all entities are not moving,
// it's better to keep this function declared but empty
// and implement it in children. that way we can avoid
// checking if the function is NULL and normally it will
// be optimized out by the compiler
static void Update(Entity *e) {}

static void Animate(Entity *e){}

static void Render(Entity *e)
{
    static bool showCollisionRect = false;
    DrawTexturePro(*(e->spriteSheet), e->frameTexture, e->destFrame, e->origin, 0, WHITE);
    if (showCollisionRect) DrawRectangleLinesEx(e->collisionRect, 2, RED);
    if (IsKeyDown(KEY_SPACE)) showCollisionRect = true; else showCollisionRect = false;
}

static bool IsMoving(Entity *e)
{
    return e->velocity.x != 0 || e->velocity.y != 0;
}

static void Free(Entity *e)
{
    int id = e->id;
    
    free(e);
    
    ENTITY_RECORD[id] = NULL;
}

/************************
 *                      *
 * Public Mass Funcs    *
 *                      *
 ************************/

void StartAll() {
    for (int i = 0; i < lastEntityID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->Update(ENTITY_RECORD[i]);
            ENTITY_RECORD[i]->Animate(ENTITY_RECORD[i]);
        }
    }
    
    SortRenderOrder();

    for (int i = 0; i < lastEntityID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->Render(ENTITY_RECORD[i]);
        }
    }
}

void FreeAll()
{
    for (int i = 0; i < lastEntityID; i++)
    {
        if (ENTITY_RECORD[i] != NULL)
        {
            ENTITY_RECORD[i]->Free(ENTITY_RECORD[i]);
        }
    }
}

/************************
 *                      *
 * Helper Funcs         *
 *                      *
 ************************/

static void SortRenderOrder()
{
    for (int i = 0; i < lastEntityID; i++)
    {
        for (int j = 0; j < lastEntityID - i - 1; j++)
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

                // printf("swapped %d and %d\n", j, j + 1);
            }
        }
    }
}