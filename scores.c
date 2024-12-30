//Author: Ainis Augustas Laurinavicius
// DATE: 2024/12/30
//Description: 2048 game's module for sorting achievements in descending order

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "2048.h"

Achievement *sortByValues(int attempts, int highScore, int currentScore, int biggestTile) {
    // CREATE STRUCTURES FOR EACH ACHIEVEMENT WITH RESPECTIVE LABELS AND VALUES
    Achievement attemptsStruct = {ATTEMPTS, attempts};
    Achievement highScoreStruct = {HIGH_SCORE_MSG, highScore};
    Achievement currentScoreStruct = {SCORE_MSG, currentScore};
    Achievement biggestTileStruct = {BIGGEST_TILE, biggestTile};

    Achievement *Achievements = (Achievement*)malloc(sizeof(Achievement)); // ALLOCATE MEMORY FOR ACHIEVEMENTS

    if (Achievements == NULL) { // CHECK IF MEMORY ALLOCATION FAILED
        free(Achievements); // FREE MEMORY IN CASE OF FAILURE
        return NULL; // RETURN NULL IF MEMORY ALLOCATION FAILED
    }

    // INITIALIZE THE ACHIEVEMENTS ARRAY
    Achievements[0] = attemptsStruct;
    Achievements = (Achievement*)realloc(Achievements, 2 * sizeof(Achievement));// INCREASE CAPACITY FOR EACH NEW ACHIEVEMENT
    Achievements[1] = highScoreStruct;
    Achievements = (Achievement*)realloc(Achievements, 3 * sizeof(Achievement));// INCREASE CAPACITY FOR EACH NEW ACHIEVEMENT
    Achievements[2] = currentScoreStruct;
    Achievements = (Achievement*)realloc(Achievements, 4 * sizeof(Achievement));// INCREASE CAPACITY FOR EACH NEW ACHIEVEMENT
    Achievements[3] = biggestTileStruct;

    if (Achievements == NULL) {
        free(Achievements);
        return NULL;
    }

    int size = 4;
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
    assert(Achievements[0].value >= Achievements[1].value); // TESTCASE 3
    return Achievements; // RETURN THE SORTED ACHIEVEMENTS
}
