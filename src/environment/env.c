#include "raylib.h"
#include "assets.h"
#include "floor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static void renderFloors(Entity *floorTile);

Entity *GetFloor(FLOORS floor) {
    static bool initialized = false;
    static Entity *floorTile;
    if (!initialized)
    {
        floorSpriteSheet = malloc(sizeof(Texture2D));
        *floorSpriteSheet = LoadTexture(ConstructPath(TEXTURES_DIR, "Floor.png"));
        
        floorTile = CreateEntity();
        floorTile->spriteSheet = floorSpriteSheet;
        floorTile->destFrame = (Rectangle){0, 0, TILE_SIZE * SCALING_FACTOR, TILE_SIZE * SCALING_FACTOR};
        floorTile->Render = renderFloors;
        
        initialized = true; 
    }
    
    
    switch (floor)
    {
        case F_NORMAL:
            floorTile->frameTexture = (Rectangle){TILE_SIZE * 1, 0, TILE_SIZE, TILE_SIZE};
            break;
    }

    return floorTile;
}

// void LoadMapArray(const char *path, int width, int height) {
//     char *mapCSV = LoadFileText(path);

//     int **mapArray = (int **)malloc(sizeof(int *) * height);
//     for (int i = 0; i < height; i++) {
//         mapArray[i] = (int *)malloc(sizeof(int) * width);
//     }

//     int row = 0, col = 0;
//     char *line = strtok(mapCSV, "\n");
//     while (line != NULL) {
//         char *cell = strtok(line, ",");
//         while (cell != NULL) {
//             mapArray[row][col] = atoi(cell);
//             cell = strtok(NULL, ",");
//             col++;
//         }
//         col = 0;
//         row++;
//         line = strtok(NULL, "\n");
//     }    

//     UnloadFileText(mapCSV);
//     return mapArray;
// }

static void renderFloors(Entity *floorTile) {
    static bool initialized = false;
    
    int width = CURRENT_LEVEL->width;
    int height = CURRENT_LEVEL->height;
    
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            floorTile->destFrame.x = col * TILE_SIZE * SCALING_FACTOR;
            floorTile->destFrame.y = row * TILE_SIZE * SCALING_FACTOR;
            DrawTexturePro(*floorTile->spriteSheet, floorTile->frameTexture, floorTile->destFrame, floorTile->origin, 0, WHITE);
        }
    }
}
