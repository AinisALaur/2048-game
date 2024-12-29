//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/29
//Description: 2048 game's module for sorting achievements in descending order

##include <stdio.h>
#include <stdlib.h>
#include "2048.h"

Achievement *sortByValues(int attempts, int highScore, int currentScore, int biggestTile) {
    // CREATE STRUCTURES FOR EACH ACHIEVEMENT WITH RESPECTIVE LABELS AND VALUES
    Achievement attemptsStruct = {ATTEMPTS, attempts};
    Achievement highScoreStruct = {HIGH_SCORE_MSG, highScore};
    Achievement currentScoreStruct = {SCORE_MSG, currentScore};
    Achievement biggestTileStruct = {BIGGEST_TILE, biggestTile};

    int size = 4; // SIZE OF THE ACHIEVEMENTS ARRAY
    Achievement *Achievements = (Achievement*)malloc(size * sizeof(Achievement)); // ALLOCATE MEMORY FOR ACHIEVEMENTS

    if (Achievements == NULL) { // CHECK IF MEMORY ALLOCATION FAILED
        free(Achievements); // FREE MEMORY IN CASE OF FAILURE
        return NULL; // RETURN NULL IF MEMORY ALLOCATION FAILED
    }

    // INITIALIZE THE ACHIEVEMENTS ARRAY
    Achievements[0] = attemptsStruct;
    Achievements[1] = highScoreStruct;
    Achievements[2] = currentScoreStruct;
    Achievements[3] = biggestTileStruct;

    // SORT THE ACHIEVEMENTS BY VALUE IN DESCENDING ORDER
    for (int i = 0; i < size; ++i) {
        for (int x = i + 1; x < size; ++x) {
            if (Achievements[i].value < Achievements[x].value) { // IF THE VALUE AT i IS LESS THAN x
                Achievement temp = Achievements[i]; // SWAP ACHIEVEMENTS[i] AND ACHIEVEMENTS[x]
                Achievements[i] = Achievements[x];
                Achievements[x] = temp;
            }
        }
    }

    return Achievements; // RETURN THE SORTED ACHIEVEMENTS
}




