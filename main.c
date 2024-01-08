#include <stdio.h>
#include "raylib.h"
#include "player.h"
#include "entity.h"
#include "assets.h"
#include "floor.h"
#include "wall.h"
#include "decor.h"
#include "ball.h"

void initGame();
void exitGame();

int main()
{
    initGame();

    SartGameCycle();

    exitGame();
    
    return 0;
}

void initGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snoopy Game");
    SetTargetFPS(FRAME_RATE);
    
    LoadTextures();
    LoadLevel(1);

}

void exitGame() {    
    UnloadCurrentLevel();
    UnloadTextures();
    CloseWindow();
}
    

