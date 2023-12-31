#include <stdio.h>
#include "raylib.h"
#include "player.h"
#include "entity.h"
#include "assets.h"
#include "floor.h"
#include "wall.h"
#include "decor.h"
#include "ball.h"

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
    SetTargetFPS(FRAME_RATE);
    
    LoadTextures();
    LoadLevel(1);

    InitFloors();
    InitWalls();
    InitDecors();
    InitBalls();
    Player *player = GetSingletonPlayer(); //initialize player
}

void startGame() {
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            SartGameCycle();
        EndDrawing();
    }
}

void exitGame() {
    StopGameCycle();
    UnloadCurrentLevel();
    UnloadTextures();
    CloseWindow();
}
    

