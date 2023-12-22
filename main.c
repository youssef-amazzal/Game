#include <stdio.h>
#include "raylib.h"
#include "player.h"
#include "entity.h"
#include "assets.h"
#include "floor.h"
#include "wall.h"
#include "decor.h"

int screenWidth = 960;
int screenHeight = 480;

void initGame();
void startGame();
void exitGame();

int main()
{
    initGame();

    startGame();

    exitGame();
    
    return 0;
}

void initGame() {
    InitWindow(screenWidth, screenHeight, "Snoopy Game");
    SetTargetFPS(120);
    
    LoadTextures();
    LoadLevel(1);

    InitFloors();
    InitWalls();
    InitDecors();
    Player *player = GetSingletonPlayer(); //initialize player

}

void startGame() {
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            StartAll();
        EndDrawing();
    }
}

void exitGame() {
    FreeAll();
    UnloadCurrentLevel();
    UnloadTextures();
    CloseWindow();
}
    

