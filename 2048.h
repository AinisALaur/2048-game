//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/27
#ifndef GAME2048_H
#define GAME2048_H
int readFromFile(int *board, int *occupiedCells, int *currentScore, int *highScore, int *initializeNewValues);
long fileSize(FILE *file);
void saveProgress(int *board, int occupiedCells, int currentScore, int highScore);
#define SQUAREAMOUNT 4
#define FILENAME "Progress.dat"
#endif
