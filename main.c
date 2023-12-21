#include <stdio.h>
#include "raylib.h"
#include "player.h"
#include "entity.h"
#include "environment/env.h"

int screenWidth = 960;
int screenHeight = 480;

int getIndex();

int main()
{
    int posX = 0;
    int posY = 0;
    InitWindow(screenWidth, screenHeight, "Snoopy Game");
    SetTargetFPS(120);

    
    Player *player = GetSingletonPlayer(); //initialize player

    
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            StartAll();
        EndDrawing();
    }

    FreeAll();
    

    CloseWindow(); // Close window and OpenGL context
    return 0;
}

