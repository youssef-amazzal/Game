#pragma once
#include "raylib.h"

typedef struct Entity
{
    int id;
    Texture2D *spriteSheet;
    Rectangle frameTexture;
    Rectangle destFrame;
    Vector2 velocity;
    float angle;
    Vector2 origin;
    void (*update)(struct Entity *e);
    int (*isMoving)(void);
} Entity;

Entity *CreateEntity(
    Texture2D *spriteSheet,
    Rectangle frameTexture,
    Rectangle destFrame,
    Vector2 origin,
    Vector2 velocity,
    int (*isMoving)(void));

void FreeEntity(Entity *e);
void ClearEntities();
void UpdateEntities();
void SetVelocity(Entity *e, Vector2 velocity, float angle);
void DrawEntities();