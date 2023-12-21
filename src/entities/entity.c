#include "entity.h"
#include "dev.h"
#include <stdlib.h>
#include <stdio.h>

Entity *ENTITY_RECORD[MAX_ENTITIES];
int ENTITY_COUNT = 0;
static int lastEntityID = 0;

static void Update(Entity *e);
static void Animate(Entity *e);
static void Render(Entity *e);
static bool IsMoving(Entity *e);


Entity *CreateEntity()
{
    Entity *e = malloc(sizeof(Entity));
    e->id = lastEntityID++;
    e->child = NULL;
    
    e->spriteSheet = NULL;
    e->frameTexture = (Rectangle){0, 0, TileSize, TileSize};
    
    e->destFrame = (Rectangle){0, 0, TileSize * SCALING_FACTOR, TileSize * SCALING_FACTOR};
    e->origin = (Vector2){0, 0};
    e->velocity = (Vector2){0, 0};
    e->angle = 0;

    e->Update = Update;
    e->Animate = Animate;
    e->Render = Render;
    e->IsMoving = IsMoving;
    
    ENTITY_RECORD[e->id] = e;
    ENTITY_COUNT++;
    return e;
}

/************************
 *                      *
 * Private Core Funcs   *
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
    DrawTexturePro(*(e->spriteSheet), e->frameTexture, e->destFrame, e->origin, 0, WHITE);
}

static bool IsMoving(Entity *e)
{
    return e->velocity.x != 0 || e->velocity.y != 0;
}

static void Free(Entity *e)
{
    int id = e->id;
    
    UnloadTexture(*(e->spriteSheet));
    free(e);
    
    ENTITY_RECORD[id] = NULL;
    ENTITY_COUNT--;
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