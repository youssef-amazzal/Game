#include "entity.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_ENTITIES 1000

static Entity *entities[MAX_ENTITIES];
static int entityCount = 0;
static int lastEntityID = 0;

static void UpdateEntity(Entity *e);

Entity *CreateEntity(
    Texture2D *spriteSheet,
    Rectangle frameTexture,
    Rectangle destFrame,
    Vector2 origin,
    Vector2 velocity,
    int (*isMoving)(void))
{
    Entity *e = malloc(sizeof(Entity));
    e->id = lastEntityID++;
    
    e->spriteSheet = spriteSheet;
    e->frameTexture = frameTexture;
    
    e->destFrame = destFrame;
    e->origin = origin;
    e->velocity = velocity;

    e->isMoving = isMoving;
    e->update = UpdateEntity;
    
    entities[e->id] = e;
    entityCount++;
    return e;
}

void FreeEntity(Entity *e)
{
    free(e);
    UnloadTexture(*(e->spriteSheet));
    entities[e->id] = NULL;
    entityCount--;
}

void ClearEntities()
{
    for (int i = 0; i < lastEntityID; i++)
    {
        if (entities[i] != NULL)
        {
            FreeEntity(entities[i]);
        }
    }
}

static void UpdateEntity(Entity *e)
{
    if (e->isMoving())
    {
        e->destFrame.x += e->velocity.x * GetFrameTime();
        e->destFrame.y += e->velocity.y * GetFrameTime();
    }
}

void UpdateEntities()
{
    for (int i = 0; i < lastEntityID; i++)
    {
        if (entities[i] != NULL)
        {
            entities[i]->update(entities[i]);
        }
    }

}

void SetVelocity(Entity *e, Vector2 velocity, float angle)
{
    e->velocity = velocity;
    e->angle = angle;
}

void DrawEntities()
{
    for (int i = 0; i < lastEntityID; i++)
    {
        if (entities[i] != NULL)
        {
            DrawTexturePro(*(entities[i]->spriteSheet), entities[i]->frameTexture, entities[i]->destFrame, entities[i]->origin, 0, RAYWHITE);
        }
    }
}
