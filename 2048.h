//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/29
//Description: 2048 game's header file

#ifndef GAME2048_H
#define GAME2048_H
#include<time.h>

int readFromFile(int *board, int *occupiedCells, int *currentScore, int *highScore, int *biggestTile);
long fileSize(FILE *file);
int saveProgress(int *board, int occupiedCells, int currentScore, int highScore, int biggestTile);

typedef struct{
    char name[30];
    int value;
}Achievement;

Achievement *sortByValues(int attempts, int highScore, int currentScore, int biggestTile);
void timeSpent();

//basic settings
#define SQUARE_AMOUNT 4
#define FILE_NAME "Progress.dat"
#define TIME_LOG_FILE "TIMES.LOG"

//printf messages
#define SCORE_MSG "SCORE: "
#define HIGH_SCORE_MSG "HIGH SCORE: "
#define ATTEMPTS "ATTEMPT: "
#define BIGGEST_TILE "ALL TIME BIGGEST TILE: "
#define BAD_MEMORY "MEMORY ALLOCATION FAILED"
#define TIME_LOG_MSG "On attempt %d total time playing in seconds: %.3lf\n"

//global variables for atexit() to work
extern clock_t start;
extern int attempts;
#endif
