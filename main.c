#include "raylib.h"
#include "player.h"
#include "entity.h"
#include <stdio.h>

int screenWidth = 800;
int screenHeight = 450;

int getIndex();

int main()
{
    int posX = 0;
    int posY = 0;
    InitWindow(screenWidth, screenHeight, "Snoopy Game");
    SetTargetFPS(120);

    Player *player = GetPlayer(); //initialize player

    while (!WindowShouldClose())
    {
        UpdateEntities();

        BeginDrawing();
            ClearBackground(WHITE);
            DrawEntities();
        EndDrawing();

    }

    ClearEntities();
    

    CloseWindow(); // Close window and OpenGL context
    return 0;
}

