#pragma once
#include "raylib.h"
#include "stdlib.h"
#include "game.h"

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
#define BALL_SPRITESHEET_PATH   TEXTURES_DIR "/" "ball.png"
#define B_TILES_PER_ROW 3
#define KEY_SPRITESHEET_PATH    TEXTURES_DIR "/" "key.png"
#define K_TILES_PER_ROW 8

#define SCALING_FACTOR 3
#define TILE_SIZE 16

extern Texture2D *playerSpriteSheet;
extern Texture2D *floorSpriteSheet;
extern Texture2D *wallSpriteSheet;
extern Texture2D *decorSpriteSheet;
extern Texture2D *ballSpriteSheet;
extern Texture2D *keySpriteSheet;

void LoadTextures();
void UnloadTextures();

Texture2D *GetKeySpriteSheet();
Texture2D *GetBallSpriteSheet();
Texture2D *GetWallSpriteSheet();
Texture2D *GetFloorSpriteSheet();
Texture2D *GetDecorSpriteSheet();
Texture2D *GetPlayerSpriteSheet();


typedef struct TextureData {
    Texture2D *(*spriteSheet)(void);
    int tilesPerRow;
    int tileTypeId; 
    HitBox hitBox;
    int zIndex;
    bool isExtension;
    bool isExtendable;
    int extensionPointsCount;
    Vector2 extensionPoints[4];
} TextureData;

/*****************
 * LEVELS
 * **************/

#define LEVEL_LAYERS_COUNT 4
typedef enum LAYERS_INDEX {
    LAYER_FLOOR = 0,
    LAYER_WALLS = 1,
    LAYER_DECOR = 2,
    LAYER_BALL = 3, 
} LAYERS_INDEX;

const static char *LAYERS[] = {
    "floor",
    "walls",
    "decor",
    "balls"
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

