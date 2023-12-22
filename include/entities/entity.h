#pragma once
#include "raylib.h"
#include "game.h"

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

    Collision collision;
    
    void (*Update)(struct Entity *e); // used normally to update the position
    void (*Render)(struct Entity *e);
    void (*Animate)();
    void (*Free)(struct Entity *e); // remember this should be implemented in the child
    
    bool (*IsMoving)();
    void (*SetDestination)(struct Entity *e, float x, float y);

} Entity;


typedef struct AssetsData {
    int tileTypeId; 
    Collision collision;
    int zIndex;
} AssetsData;


#define MAX_ENTITIES 400
extern Entity *ENTITY_RECORD[];

Entity *CreateEntity();

void StartAll();
void FreeAll();