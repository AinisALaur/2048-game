//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/29
//Description: 2048 game's module for reading and writing from/to files

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "2048.h"

#define EXPECTED_FILE_SIZE SQUARE_AMOUNT * SQUARE_AMOUNT + 5

long fileSize(FILE *file) {
    if (!file) {
        return -1; // RETURN -1 IF FILE IS NULL
    } else {
        long currentPos = ftell(file); // STORE CURRENT POSITION IN FILE
        fseek(file, 0, SEEK_END); // MOVE FILE POINTER TO END
        long size = ftell(file); // GET FILE SIZE
        fseek(file, currentPos, SEEK_SET); // RESTORE FILE POINTER TO ORIGINAL POSITION
        return size; // RETURN FILE SIZE
    }
}

int readFromFile(int *board, int *occupiedCells, int *currentScore, int *highScore, int *biggestTile) {
    if (board != NULL && occupiedCells != NULL && currentScore != NULL && highScore != NULL && biggestTile != NULL) {
        FILE *progressFile = fopen(FILE_NAME, "rb"); // OPEN FILE IN READ-BINARY MODE
        FILE *nonExistentFile = fopen("", "rb");
        assert(fileSize(nonExistentFile) == -1); // TESTCASE 2
        if (progressFile != NULL && fileSize(progressFile) / sizeof(int) == EXPECTED_FILE_SIZE) { // CHECK IF EXPECTED NUMBER OF INFO IS INSIDE
            fread(board, sizeof(int), SQUARE_AMOUNT * SQUARE_AMOUNT, progressFile); // READ BOARD DATA
            fread(occupiedCells, sizeof(int), 1, progressFile); // READ OCCUPIED CELLS
            fread(currentScore, sizeof(int), 1, progressFile); // READ CURRENT SCORE
            fread(highScore, sizeof(int), 1, progressFile); // READ HIGH SCORE
            fread(&attempts, sizeof(int), 1, progressFile); // READ ATTEMPTS
            fread(biggestTile, sizeof(int), 1, progressFile); // READ BIGGEST TILE
            fclose(progressFile); // CLOSE FILE
            return 1; // RETURN SUCCESS
        } else {
            return -1; // RETURN FAILURE IF FILE SIZE IS NOT AS EXPECTED
        }
    } else {
        return -1; // RETURN FAILURE IF POINTERS ARE NULL
    }
}

int saveProgress(int *board, int occupiedCells, int currentScore, int highScore, int biggestTile) {
    if (board != NULL) {
        FILE *progressFile = fopen(FILE_NAME, "wb"); // OPEN FILE IN WRITE-BINARY MODE
        if (progressFile != NULL) {
            fwrite(board, sizeof(int), SQUARE_AMOUNT * SQUARE_AMOUNT, progressFile); // WRITE BOARD DATA
            fwrite(&occupiedCells, sizeof(int), 1, progressFile); // WRITE OCCUPIED CELLS
            fwrite(&currentScore, sizeof(int), 1, progressFile); // WRITE CURRENT SCORE
            fwrite(&highScore, sizeof(int), 1, progressFile); // WRITE HIGH SCORE
            fwrite(&attempts, sizeof(int), 1, progressFile); // WRITE ATTEMPTS
            fwrite(&biggestTile, sizeof(int), 1, progressFile); // WRITE BIGGEST TILE
            fclose(progressFile); // CLOSE FILE
            return 1; // RETURN SUCCESS
        } else {
            return -1; // RETURN FAILURE IF FILE COULD NOT BE OPENED
        }
    }
    return -1; // RETURN FAILURE IF BOARD POINTER IS NULL
}

void timeSpent() {
    int end = clock(); // GET END TIME
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; // CALCULATE TIME TAKEN
    char message[] = TIME_LOG_MSG;
    FILE *file = fopen(TIME_LOG_FILE, "a"); // OPEN TIME LOG FILE IN APPEND MODE
    if (file != NULL) {
        fprintf(file, TIME_LOG_MSG, attempts, time_taken); // LOG TIME SPENT ON ATTEMPT
        fclose(file); // CLOSE FILE
    }
}
