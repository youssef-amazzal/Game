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
    bool isReactive;
    
    void (*Update)(struct Entity *e); // used normally to update the position
    void (*Animate)(struct Entity *e);
    void (*Render)(struct Entity *e);
    void (*React)(struct Entity *e);
    void (*Free)(struct Entity *e); // remember this should be implemented in the child
    
    bool (*IsMoving)();
    void (*SetDestination)(struct Entity *e, float x, float y);
    void (*GetCollisions)(struct Entity *e);
    bool (*CheckCollision)(struct Entity *e1, struct Entity *e2);

} Entity;


extern Entity *ENTITY_RECORD[];
extern int LAST_ID;

Entity *CreateEntity();