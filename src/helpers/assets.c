#include "assets.h"
#include "raylib.h"
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>

Texture2D *playerSpriteSheet;
Texture2D *floorSpriteSheet;
Texture2D *wallSpriteSheet;
Texture2D *decorSpriteSheet;
Texture2D *ballSpriteSheet;
Texture2D *keySpriteSheet;

Level *CURRENT_LEVEL = NULL;
Level LEVELS[LEVELS_COUNT] = {
    {
        .num = 1,
        .width = 20, 
        .height = 10,
    }
};

// Path funcs
const char *ConstructPath(const char *str1, const char *str2) {
    return TextFormat("%s/%s", str1, str2);
}

// Texture funcs
void LoadTextures() {
    playerSpriteSheet = malloc(sizeof(Texture2D));
    *playerSpriteSheet = LoadTexture(PLAYER_SPRITESHEET_PATH);

    floorSpriteSheet = malloc(sizeof(Texture2D));
    *floorSpriteSheet = LoadTexture(FLOOR_SPRITESHEET_PATH);

    wallSpriteSheet = malloc(sizeof(Texture2D));
    *wallSpriteSheet = LoadTexture(WALL_SPRITESHEET_PATH);

    decorSpriteSheet = malloc(sizeof(Texture2D));
    *decorSpriteSheet = LoadTexture(DECOR_SPRITESHEET_PATH);

    ballSpriteSheet = malloc(sizeof(Texture2D));
    *ballSpriteSheet = LoadTexture(BALL_SPRITESHEET_PATH);

    keySpriteSheet = malloc(sizeof(Texture2D));
    *keySpriteSheet = LoadTexture(KEY_SPRITESHEET_PATH);
}

void UnloadTextures() {
    UnloadTexture(*playerSpriteSheet);
    UnloadTexture(*floorSpriteSheet);
    UnloadTexture(*wallSpriteSheet);
    UnloadTexture(*decorSpriteSheet);
    UnloadTexture(*ballSpriteSheet);
    UnloadTexture(*keySpriteSheet);

    free(playerSpriteSheet);
    free(floorSpriteSheet);
    free(wallSpriteSheet);
    free(decorSpriteSheet);
    free(ballSpriteSheet);
    free(keySpriteSheet);
}

// level funcs
int **LoadLayerArray(Level *level, LAYERS_INDEX layerIndex) {

    const char *fileName = TextFormat("level_%d_%s.csv", level->num, LAYERS[layerIndex]);
    const char *path = ConstructPath(LEVELS_DIR, fileName);
    
    char *layerCSV = LoadFileText(path);

    int **LayerArray = malloc(sizeof(int**) * level->height);
    for (int i = 0; i < level->height; i++) {
        LayerArray[i] = (int *)malloc(sizeof(int) * level->width);
    }

    int row = 0, col = 0;
    char *saveptr1, *saveptr2;
    char *line = strtok_r(layerCSV, "\n", &saveptr1);
    while (line != NULL) {
        char *cell = strtok_r(line, ",", &saveptr2);
        while (cell != NULL) {
            LayerArray[row][col] = atoi(cell);
            cell = strtok_r(NULL, ",", &saveptr2);
            col++;
        }
        col = 0;
        row++;
        line = strtok_r(NULL, "\n", &saveptr1);
    }


    UnloadFileText(layerCSV);
    return LayerArray;
}

void LoadLevel(int levelNum) {
    if (CURRENT_LEVEL != NULL) {
        UnloadCurrentLevel();
    }

    CURRENT_LEVEL = &LEVELS[levelNum - 1];
    
    for (int i = 0; i < LEVEL_LAYERS_COUNT; i++) {
        CURRENT_LEVEL->layers[i] = LoadLayerArray(CURRENT_LEVEL, i);
    }
}

void UnloadCurrentLevel() {
    for (int i = 0; i < LEVEL_LAYERS_COUNT; i++) {
        for (int j = 0; j < CURRENT_LEVEL->height; j++) {
            free(CURRENT_LEVEL->layers[i][j]);
        }
        free(CURRENT_LEVEL->layers[i]);
    }
    CURRENT_LEVEL = NULL;
}

Texture2D *GetKeySpriteSheet() {
    return keySpriteSheet;
}
Texture2D *GetBallSpriteSheet() {
    return ballSpriteSheet;
}
Texture2D *GetWallSpriteSheet() {
    return wallSpriteSheet;
}
Texture2D *GetFloorSpriteSheet() {
    return floorSpriteSheet;
}
Texture2D *GetDecorSpriteSheet() {
    return decorSpriteSheet;
}
Texture2D *GetPlayerSpriteSheet() {
    return playerSpriteSheet;
}

