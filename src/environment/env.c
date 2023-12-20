#include "raylib.h"
#include "env.h"
#include <string.h>
#include <stdlib.h>

int **LoadMapArray(char *path, Map level) {
    char *mapCSV = LoadFileText(path);

    int **mapArray = (int **)malloc(sizeof(int *) * level.height);
    for (int i = 0; i < level.height; i++) {
        mapArray[i] = (int *)malloc(sizeof(int) * level.width);
    }

    int row = 0, col = 0;
    char *line = strtok(mapCSV, "\n");
    while (line != NULL) {
        char *cell = strtok(line, ",");
        while (cell != NULL) {
            mapArray[row][col] = atoi(cell);
            cell = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
        line = strtok(NULL, "\n");
    }    

    UnloadFileText(mapCSV);
    return mapArray;
}
