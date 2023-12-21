#pragma once
#include "raylib.h"
#include "stdlib.h"

/*****************
 * PATHS
 * **************/

const char *ConstructPath(const char *str1, const char *str2);

#define ASSETS_DIR      "assets"
#define TEXTURES_DIR    ASSETS_DIR "/" "textures"
#define SOUNDS_DIR      ASSETS_DIR "/" "sounds"
#define FONTS_DIR       ASSETS_DIR "/" "fonts"
#define LEVELS_DIR      ASSETS_DIR "/" "levels"

/*****************
 * TEXTURES
 * **************/

#define PLAYER_SPRITESHEET_PATH TEXTURES_DIR "/" "player.png"
#define FLOOR_SPRITESHEET_PATH  TEXTURES_DIR "/" "floor.png"
#define F_TILES_PER_ROW 2
#define WALL_SPRITESHEET_PATH   TEXTURES_DIR "/" "wall.png"
#define W_TILES_PER_ROW 9
#define DECOR_SPRITESHEET_PATH  TEXTURES_DIR "/" "decor.png"
#define D_TILES_PER_ROW 9

#define SCALING_FACTOR 3
#define TILE_SIZE 16

extern Texture2D *playerSpriteSheet;
extern Texture2D *floorSpriteSheet;
extern Texture2D *wallSpriteSheet;
extern Texture2D *decorSpriteSheet;

void LoadTextures();
void UnloadTextures();

/*****************
 * LEVELS
 * **************/

#define LEVEL_LAYERS_COUNT 3
typedef enum LAYERS_INDEX {
    LAYER_FLOOR = 0,
    LAYER_WALLS = 1,
    LAYER_DECOR = 2
} LAYERS_INDEX;

const static char *LAYERS[] = {
    "floor",
    "walls",
    "decor"
};


typedef struct Level {
    int num;
    int width;
    int height;
    int **layers[LEVEL_LAYERS_COUNT];
} Level;

#define LEVELS_COUNT 1
extern Level LEVELS[];


int **LoadLayerArray(Level *level, LAYERS_INDEX layer);
void LoadLevel(int levelNum);
void UnloadCurrentLevel();

extern Level *CURRENT_LEVEL;
#define LEVEL_NUMBERS 1

