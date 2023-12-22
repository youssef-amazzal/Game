#pragma once
#include "raylib.h"

typedef struct Entity
{
    int id;
    void *child;
    bool active;

    Texture2D *spriteSheet;
    Rectangle frameTexture;

    int zIndex;

    Rectangle destFrame;
    Vector2 velocity;
    float angle;
    Vector2 origin;

    Rectangle collisionRect;
    
    void (*Update)(struct Entity *e); // used normally to update the position
    void (*Render)(struct Entity *e);
    void (*Animate)();
    void (*Free)(struct Entity *e); // remember this should be implemented in the child
    bool (*IsMoving)();
} Entity;

typedef Rectangle StaticCollisionRect;

// a lazy way to avoid JSON parsing
typedef struct AssetsData {
    int tileTypeId; 
    StaticCollisionRect collisionRect;
    int zIndex;
} AssetsData;

#define MAX_ENTITIES 400
extern Entity *ENTITY_RECORD[];

Entity *CreateEntity();

void StartAll();
void FreeAll();