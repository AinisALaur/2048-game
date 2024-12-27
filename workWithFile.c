//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/27
#include<stdio.h>
#include "2048.h"

long fileSize(FILE *file) {
    if (!file) {
        return 0;
    }else{
        long currentPos = ftell(file);
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, currentPos, SEEK_SET);
        return size;
    }
}


int readFromFile(int *board, int *occupiedCells, int *currentScore, int *highScore, int *initializeNewValues){
    FILE* progressFile = fopen(FILENAME, "rb");
    if (progressFile != NULL && fileSize(progressFile) / sizeof(int) == SQUAREAMOUNT * SQUAREAMOUNT + 3) {
        fread(board, sizeof(int), SQUAREAMOUNT * SQUAREAMOUNT, progressFile);
        fread(occupiedCells, sizeof(int), 1, progressFile);
        fread(currentScore, sizeof(int), 1, progressFile);
        fread(highScore, sizeof(int), 1, progressFile);
        fclose(progressFile);
        *initializeNewValues = 0;
        return 1;
    }
    return 0;
}


void saveProgress(int *board, int occupiedCells, int currentScore, int highScore){
    if(board != NULL){
        FILE* progressFile = fopen(FILENAME, "wb");
        if (progressFile != NULL) {
            fwrite(board, sizeof(int), SQUAREAMOUNT * SQUAREAMOUNT, progressFile);
            fwrite(&occupiedCells, sizeof(int), 1, progressFile);
            fwrite(&currentScore, sizeof(int), 1, progressFile);
            fwrite(&highScore, sizeof(int), 1, progressFile);
            fclose(progressFile);
        }
    }
}
