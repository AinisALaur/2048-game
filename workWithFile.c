//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/27
#include<stdio.h>
#include<time.h>
#include "2048.h"

long fileSize(FILE *file) {
    if (!file) {
        return -1;
    }else{
        long currentPos = ftell(file);
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, currentPos, SEEK_SET);
        return size;
    }
}


int readFromFile(int *board, int *occupiedCells, int *currentScore, int *highScore, int *initializeNewValues, int *biggestTile){
    if(board != NULL && occupiedCells != NULL && currentScore != NULL && highScore != NULL && initializeNewValues != NULL && biggestTile != NULL){
        FILE* progressFile = fopen(FILE_NAME, "rb");
        if (progressFile != NULL && fileSize(progressFile) / sizeof(int) == SQUARE_AMOUNT * SQUARE_AMOUNT + 5) {
            fread(board, sizeof(int), SQUARE_AMOUNT * SQUARE_AMOUNT, progressFile);
            fread(occupiedCells, sizeof(int), 1, progressFile);
            fread(currentScore, sizeof(int), 1, progressFile);
            fread(highScore, sizeof(int), 1, progressFile);
            fread(&attempts, sizeof(int), 1, progressFile);
            fread(biggestTile, sizeof(int), 1, progressFile);
            fclose(progressFile);
            *initializeNewValues = 0;
            return 1;
        }
        else
            return -1;
    }
    else
        return -1;
}


int saveProgress(int *board, int occupiedCells, int currentScore, int highScore, int biggestTile){
    if(board != NULL){
        FILE* progressFile = fopen(FILE_NAME, "wb");
        if (progressFile != NULL) {
            fwrite(board, sizeof(int), SQUARE_AMOUNT * SQUARE_AMOUNT, progressFile);
            fwrite(&occupiedCells, sizeof(int), 1, progressFile);
            fwrite(&currentScore, sizeof(int), 1, progressFile);
            fwrite(&highScore, sizeof(int), 1, progressFile);
            fwrite(&attempts, sizeof(int), 1, progressFile);
            fwrite(&biggestTile, sizeof(int), 1, progressFile);
            fclose(progressFile);
            return 1;
        }
        else
            return -1;
    }
    return -1;
}

void timeSpent() {
    int end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    char message[] = TIME_LOG_MSG;
    FILE *file = fopen(TIME_LOG_FILE, "a");
    if (file != NULL) {
        fprintf(file, TIME_LOG_MSG, attempts, time_taken);
        fclose(file);
    }
}
