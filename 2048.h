//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/27
#ifndef GAME2048_H
#define GAME2048_H
int readFromFile(int *board, int *occupiedCells, int *currentScore, int *highScore, int *initializeNewValues, int *attempts, int *biggestTile);
long fileSize(FILE *file);
void saveProgress(int *board, int occupiedCells, int currentScore, int highScore, int attempts, int biggestTile);
typedef struct{
    char name[20];
    int value;
}Achievement;
Achievement *sortByValues(int attempts, int highScore, int currentScore, int biggestTile);
#define SQUAREAMOUNT 4
#define FILENAME "Progress.dat"
#define SCOREMSG "SCORE: "
#define HIGHSCOREMSG "HIGH SCORE: "
#define ATTEMPTS "ATTEMP: "
#define BIGGESTTILE "BIGGEST TILE: "
#define BADMEMORY "MEMORY ALLOCATION FAILED"
#endif
