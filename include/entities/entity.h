#pragma once
#include "raylib.h"
#include "game.h"
#include "assets.h"

typedef struct Entity
{
    int id;
    void *child;
    bool active;

    int extensionCount;
    void **extensions;

    Texture2D *spriteSheet;
    Rectangle frameTexture;

    int zIndex;

    Rectangle destFrame;
    Vector2 velocity;
    float angle;
    Vector2 origin;

    HitBox hitBox;
    
    void (*Update)(struct Entity *e); // used normally to update the position
    void (*Animate)(struct Entity *e);
    void (*Render)(struct Entity *e);
    void (*React)(struct Entity *e);
    void (*Free)(struct Entity *e); // remember this should be implemented in the child
    
    bool (*IsMoving)();
    void (*SetDestination)(struct Entity *e, float x, float y);
    void (*GetHitBoxs)(struct Entity *e);
    bool (*CheckHitBox)(struct Entity *e1, struct Entity *e2);

} Entity;


extern Entity *ENTITY_RECORD[];
extern int LAST_ID;

Entity *CreateEntity();
void SetTexturesData(Entity *e, TextureData textureData);
void MoveHitBox(int id, TextureData textureData);
