//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/27
#include<stdio.h>
#include<stdlib.h>
#include "2048.h"

Achievement *sortByValues(int attempts, int highScore, int currentScore, int biggestTile){
    Achievement attemptsStruct = {ATTEMPTS, attempts};
    Achievement highScoreStruct = {HIGH_SCORE_MSG, highScore};
    Achievement currentScoreStruct = {SCORE_MSG, currentScore};
    Achievement biggestTileStruct = {BIGGEST_TILE, biggestTile};


    int size = 4;
    Achievement *Achievements = (Achievement*)malloc(size * sizeof(Achievement));
    if(Achievements == NULL){
        printf(BAD_MEMORY);
        exit(0);
    }
    Achievements[0] = attemptsStruct;
    Achievements[1] = highScoreStruct;
    Achievements[2] = currentScoreStruct;
    Achievements[3] = biggestTileStruct;

    for(int i=0; i<size; ++i){
        for(int x=i+1; x<size; ++x){
            if(Achievements[i].value<Achievements[x].value){
                Achievement temp = Achievements[i];
                Achievements[i] = Achievements[x];
                Achievements[x] = temp;
            }
        }
    }

    return Achievements;
}




