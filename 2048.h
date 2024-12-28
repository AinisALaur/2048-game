//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/27
#ifndef GAME2048_H
#define GAME2048_H
#include<time.h>
int readFromFile(int *board, int *occupiedCells, int *currentScore, int *highScore, int *initializeNewValues, int *attempts, int *biggestTile);
long fileSize(FILE *file);
void saveProgress(int *board, int occupiedCells, int currentScore, int highScore, int attempts, int biggestTile);
typedef struct{
    char name[20];
    int value;
}Achievement;
Achievement *sortByValues(int attempts, int highScore, int currentScore, int biggestTile);
void timeSpent();
#define SQUARE_AMOUNT 4
#define FILE_NAME "Progress.dat"
#define SCORE_MSG "SCORE: "
#define HIGH_SCORE_MSG "HIGH SCORE: "
#define ATTEMPTS "ATTEMP: "
#define BIGGEST_TILE "BIGGEST TILE: "
#define BAD_MEMORY "MEMORY ALLOCATION FAILED"
#define TIME_LOG_FILE "TIMES.LOG"
#define TIME_LOG_MSG "Time spent playing in seconds: "
extern clock_t start;
#endif
