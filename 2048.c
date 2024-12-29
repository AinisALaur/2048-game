//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/29

#include<stdio.h>
#include<stdlib.h> // read and write files
#include<time.h> //to get random values for board
#include<string.h> //for mem functions
#include <windows.h> //to handle unexpected program shutdown
#include <conio.h> // Track keyboard events
#include <ctype.h> //for function toupper()
#include "2048.h" //HEADER FILE

//Basic settings
#define SQUARE_SIZE 11 //tested with 11
#define VALUE_COLOR  "\x1B[36m" //cyan
#define ACHIEVEMENT_COLOR  "\x1B[33m" //yellow
#define HIGHLIGHT_COLOR  "\x1B[32m" //green
#define BAD_INPUT_COLOR  "\x1B[31m" //red
#define COLOR_RESET  "\x1B[37m" //white

//PRINTF messages
#define FOUND_PROGRESS_MSG "Game progress found!\n"
#define NEW_GAME_MSG "Starting a new game!\n"
#define GAME_ERROR_MSG "SOMETHING WENT WRONG CHECK MEMORY ALLOCATION OR CELL SIZE!"
#define GAME_END_MSG "Game has ended"
#define BAD_INPUT_MSG "BAD INPUT"
#define CONTROL_UP_MSG "UP"
#define CONTROL_DOWN_MSG "DOWN"
#define CONTROL_LEFT_MSG "LEFT"
#define CONTROL_RIGHT_MSG "RIGHT"
#define CONTROL_SAVE_GAME_MSG "SAVE GAME"
#define CONTROL_NEW_GAME_MSG "NEW GAME"

//Controls for the game
#define UP 'W'
#define DOWN 'S'
#define LEFT 'A'
#define RIGHT 'D'
#define SAVE_GAME 'E'
#define START_NEW_GAME 'N'

//Global variables to make it work with atexit()
clock_t start;
int attempts;


//get number size to align to center when printing
int getNumSize(int value){
    int t = 0;
    while(value > 0){
        t++;
        value /= 10;
    }
    return t;
}

//Check if no more available moves
int endGame(int *board) {
    if(board != NULL){
        for (int i = 0; i < SQUARE_AMOUNT; ++i) {
            for (int x = 0; x < SQUARE_AMOUNT; ++x) {
                int current = *(board + SQUARE_AMOUNT * i + x);

                // Check left neighbor
                if (x > 0 && current == *(board + SQUARE_AMOUNT * i + (x - 1))) {
                    return 0;
                }

                // Check right neighbor
                if (x < SQUARE_AMOUNT-1 && current == *(board + SQUARE_AMOUNT * i + (x + 1))) {
                    return 0;
                }

                // Check top neighbor
                if (i > 0 && current == *(board + SQUARE_AMOUNT * (i - 1) + x)) {
                    return 0;
                }

                // Check bottom neighbor
                if (i < SQUARE_AMOUNT-1 && current == *(board + SQUARE_AMOUNT * (i + 1) + x)) {
                    return 0;
                }
            }
        }
        return 1;
    }
    return -1;
}

// print a straight horizontal line for board visualization
void drawLine(int length){
    for(int i =0; i < SQUARE_AMOUNT; ++i){
        for(int x = 0; x < length; ++x){
            printf("-", x);
        }
    }printf("-\n");
}

//draw game board
int drawBoard(int *board, int newValueX, int newValueY, int currentScore, int badInput, int highScore, int biggestTile){
    if(board != NULL){
        for(int j = 0; j < SQUARE_AMOUNT; ++j){
            drawLine(SQUARE_SIZE);
            for(int i = 0; i < SQUARE_SIZE / 2; ++i){
                for(int x = 0; x < SQUARE_AMOUNT; ++x){
                    if(i == SQUARE_SIZE / 4 && *(board + SQUARE_AMOUNT * j + x) != 0){

                        //calculate padding to align number to center
                        int cellWidth = SQUARE_SIZE - 1;
                        int sizeOfNum = getNumSize(*(board + SQUARE_AMOUNT * j + x));
                        int leftPadding = (cellWidth - sizeOfNum) / 2;
                        int rightPadding = cellWidth - leftPadding - sizeOfNum;

                        if(cellWidth < sizeOfNum){
                            return -1; // if text overflow occurs
                        }

                        char cellValueColor[] = VALUE_COLOR;
                        if(j == newValueY && x == newValueX)
                            strcpy(cellValueColor, HIGHLIGHT_COLOR);

                        printf("|%s%*s%d%s%*s", cellValueColor, leftPadding, "", *(board + SQUARE_AMOUNT * j + x), COLOR_RESET, rightPadding, "");
                    }
                    else{
                        printf("|%*s", SQUARE_SIZE - 1, "");
                    }
                }

                Achievement *Achievements = (Achievement*)malloc(4 * sizeof(Achievement));
                if(Achievements == NULL){
                    return -1; // if memory allocation failed
                }

                Achievements = sortByValues(attempts, highScore, currentScore, biggestTile); //sort Achievements in descending order
                if(Achievements == NULL){ //check if memory allocation was successful inside function
                    return -1;
                }

                if(j == 0 && i == 0)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[0].name, Achievements[0].value, COLOR_RESET);
                else if(j == 0 && i == 1)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[1].name, Achievements[1].value, COLOR_RESET);
                else if(j == 0 && i == 2)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[2].name, Achievements[2].value, COLOR_RESET);
                else if(j == 0 && i == 3)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[3].name, Achievements[3].value, COLOR_RESET);
                else if(j == 2 && i == 1 && badInput)
                    printf("|  %s%s%s\n", BAD_INPUT_COLOR, BAD_INPUT_MSG, COLOR_RESET);
                else
                    printf("|\n");
                free(Achievements);
            }
        }
        drawLine(SQUARE_SIZE);
        return 1;
    }
    else
        return -1;
}

//Adds initial values and extra value after a valid move
int initializeNewValue(int *board, int NumberOfValues, int *occupiedCells, int *newValueX, int *newValueY, int *biggestTile){
    if(board != NULL && newValueX != NULL && newValueY != NULL && newValueX != NULL && biggestTile != NULL){
        int x, y;
        for(int i = 0; i < NumberOfValues; ++i){
            int IsOccupied = 1;
            while(IsOccupied){
                x = rand() % SQUARE_AMOUNT;
                y = rand() % SQUARE_AMOUNT;

                if(*(board + SQUARE_AMOUNT * y + x) == 0)
                    IsOccupied = 0;
            }
            int newCellValue[10] = {4, 2, 2, 2, 2, 2, 2, 2, 2, 2}; // 4 has a 10% to appear
            int newIndex = rand() % 10;
            *(board + SQUARE_AMOUNT * y + x) = newCellValue[newIndex];
            *biggestTile = (*biggestTile) > newCellValue[newIndex]? *biggestTile: newCellValue[newIndex];
            (*occupiedCells) += 1;
            *newValueX = x;
            *newValueY = y;
        }
        return 1;
    }
    else
        return -1;
}

int moveVertically(int *board, int *occupiedCells, char direction, int *newValueX, int *newValueY, int *currentScore, int *biggestTile){
    if(board != NULL && occupiedCells != NULL && newValueX != NULL && newValueY != NULL && currentScore != NULL && biggestTile != NULL){
        int *movedCells = (int *)calloc(SQUARE_AMOUNT * SQUARE_AMOUNT, sizeof(int));
        if(movedCells == NULL){
            free(movedCells);
            return -1;
        }
        int y = direction == UP ? SQUARE_AMOUNT - 1: 0;

        int *initialBoard = (int *)malloc(SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));
        if(initialBoard == NULL){
            free(initialBoard);
            return -1;
        }
        memcpy(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));

        while(1){ //starts at bottom row and progresses up

            //check when to stop cycle
            if(direction == UP && y <= 0 || direction == DOWN && y >= SQUARE_AMOUNT - 1){
                break;
            }

            for(int x = 0; x < SQUARE_AMOUNT; ++x){ //goes trough row's elements
                if(*(board + SQUARE_AMOUNT * y + x) != 0){ // if element is not 0 we look if any non 0 elements exist above/below

                    //assign j accordingly
                    int  y1 = direction == UP? y - 1: y + 1;

                    while(1){ // loop that checks all elements above/below
                        if(direction == UP && y1 < 0 || direction == DOWN && y1 > SQUARE_AMOUNT - 1){
                            break;
                        }

                        //if finds the same element above it
                        if(*(board + SQUARE_AMOUNT * y1 + x) != 0 && *(board + SQUARE_AMOUNT * y1 + x) == *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0){
                           *currentScore += (*(board + SQUARE_AMOUNT * y1 + x)) * 2;
                           *(board + SQUARE_AMOUNT * y1 + x) = (*(board + SQUARE_AMOUNT * y1 + x)) * 2;
                           *(board + SQUARE_AMOUNT * y + x) = 0;
                           *biggestTile = (*biggestTile) > *(board + SQUARE_AMOUNT * y1 + x)? *biggestTile: *(board + SQUARE_AMOUNT * y1 + x);
                           (*occupiedCells)--;
                            *(movedCells + SQUARE_AMOUNT * y1 + x) = 1; // make sure to not move already moved values
                            break;
                        }

                        //if finds a non 0 element that is not equal to original
                        else if(*(board + SQUARE_AMOUNT * y1 + x) != 0 && *(board + SQUARE_AMOUNT * y1 + x) != *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0){
                           int new_value = *(board + SQUARE_AMOUNT * y + x);
                           *(board + SQUARE_AMOUNT * y + x) = 0;
                           int Yvalue = direction == UP? y1 + 1: y1 - 1;

                           *(board + SQUARE_AMOUNT * Yvalue + x) = new_value;
                           *(movedCells + SQUARE_AMOUNT * Yvalue + x) = 1; // make sure to not move already moved values
                           break;
                        }

                        //move to upper or lower row
                        direction == UP ? --y1 : ++y1;
                    }
                }
            }

            //move up or down
            direction == UP ? --y : ++y;
        }

        //assign appropriate i value for upcoming loop
        y = direction == UP ? 1 : SQUARE_AMOUNT - 2;

        //Move all elements up/down
        while(1){ // start at second row from top and progress down or second row from bottom

            //break function when needed
            if(direction == UP && y > SQUARE_AMOUNT - 1 || direction == DOWN && y < 0){
                break;
            }

            for(int x = 0; x < SQUARE_AMOUNT; ++x){ // go through all row's elements
                int cellValue = *(board + SQUARE_AMOUNT * y + x);
                if(cellValue != 0){ // if element is not zero move it as high/low up as possible
                    *(board + SQUARE_AMOUNT * y + x) = 0;
                    int ValuesYcord;
                    if(direction == UP){
                        ValuesYcord = 0;
                        while(*(board + SQUARE_AMOUNT * ValuesYcord + x) != 0) // increasing it's y coordinate to the maximum
                            ValuesYcord++;
                    }
                    else{
                        ValuesYcord = SQUARE_AMOUNT - 1;
                        while(*(board + SQUARE_AMOUNT * ValuesYcord + x) != 0) // decreasing it's y coordinate to the maximum
                            ValuesYcord--;
                    }

                    *(board + SQUARE_AMOUNT * ValuesYcord + x) = cellValue;
                }
            }
           // move up a row or down
           direction == UP ? ++y : --y;
        }

        if(memcmp(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int)) != 0)
            initializeNewValue(board, 1, occupiedCells, newValueX, newValueY, biggestTile); // add new value to board
        free(movedCells);
        free(initialBoard);
        return 1;
    }
    else
        return -1;
}

int moveHorizontally(int *board, int *occupiedCells, char direction, int *newValueX, int *newValueY, int *currentScore, int *biggestTile){
    if(board != NULL && occupiedCells != NULL && newValueX != NULL && newValueY != NULL && currentScore != NULL && biggestTile != NULL){
        int *movedCells = (int *)calloc(SQUARE_AMOUNT * SQUARE_AMOUNT, sizeof(int));
        if(movedCells == NULL){
            free(movedCells);
            return -1;
        }


        int x = direction == LEFT ? SQUARE_AMOUNT - 1 : 0;

        int *initialBoard = (int *)malloc(SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));
        if(initialBoard == NULL){
            free(initialBoard);
            return -1;
        }

        memcpy(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));


        //move left or right
        while(1){

            if(direction == LEFT && x < 1 || direction == RIGHT && x > SQUARE_AMOUNT - 1)
                break;

            for(int y = 0; y < SQUARE_AMOUNT; ++y){
                if(*(board + SQUARE_AMOUNT * y + x) != 0){
                    int x1 = direction == LEFT? x - 1: x + 1;
                    while(1){
                        if(direction == LEFT && x1 < 0 || direction == RIGHT && x1 > SQUARE_AMOUNT - 1)
                            break;

                        if(*((int *)board + SQUARE_AMOUNT * y + x1) != 0 && *(board + SQUARE_AMOUNT * y + x1) == *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0){
                           *currentScore += (*(board + SQUARE_AMOUNT * y + x)) * 2;
                           *(board + SQUARE_AMOUNT * y + x1) = (*(board + SQUARE_AMOUNT * y + x)) * 2;
                           *biggestTile = (*biggestTile) > *(board + SQUARE_AMOUNT * y + x1)? *biggestTile: *(board + SQUARE_AMOUNT * y + x1);
                           *(board + SQUARE_AMOUNT * y + x) = 0;
                           (*occupiedCells)--;
                           *(movedCells + SQUARE_AMOUNT * y + x1) = 1;
                            break;
                        }


                        if(*(board + SQUARE_AMOUNT * y + x1) != 0 && *(board + SQUARE_AMOUNT * y + x1) != *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0){
                           int value = *(board + 4 * y + x);
                           *(board + SQUARE_AMOUNT * y + x) = 0;
                           int Xvalue = direction == LEFT? x1 + 1: x1 - 1;

                           *(board + SQUARE_AMOUNT * y + Xvalue) = value;
                           *(movedCells + SQUARE_AMOUNT * y + Xvalue) = 1;
                           break;
                        }

                        direction == LEFT ? --x1 : ++x1;
                    }
                }

            }

            direction == LEFT ? --x : ++x;

         }

         x = direction == LEFT ? 1 : SQUARE_AMOUNT - 2;

         while(1){
            if(direction == LEFT && x > SQUARE_AMOUNT - 1 || direction == RIGHT && x < 0)
                break;

            for(int y = 0; y < SQUARE_AMOUNT; ++y){
                if(*(board + SQUARE_AMOUNT * y + x) != 0){
                    int value = *(board + SQUARE_AMOUNT * y + x);
                    *(board + SQUARE_AMOUNT * y + x) = 0;

                    int xcor;

                    if(direction == LEFT){
                        xcor = 0;
                        //decrease x coordinate to the maximum (move left)
                        while(*(board + SQUARE_AMOUNT * y + xcor) != 0 && xcor < SQUARE_AMOUNT - 1){
                            xcor++;
                        }
                    }

                    if(direction == RIGHT){
                        xcor = SQUARE_AMOUNT - 1;
                        //increase x coordinate to the maximum (move left)
                            while(*(board + SQUARE_AMOUNT * y + xcor) != 0 && xcor > 0){
                                xcor--;
                            }
                        }

                    *((int *)board + SQUARE_AMOUNT * y + xcor) = value;
                }
            }
            direction == LEFT ? ++x : --x;
         }

        if(memcmp(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int)) != 0)
            initializeNewValue(board, 1, occupiedCells, newValueX, newValueY, biggestTile); // add new value to board
        free(movedCells);
        free(initialBoard);
        return 1;
    }
    else
        return -1;
}

int startNewGame(int *board, int *occupiedCells, int *currentScore, int *newValueX, int *newValueY, int *biggestTile){
    if(board != NULL && occupiedCells != NULL && currentScore != NULL && newValueX != NULL && newValueY != NULL && biggestTile != NULL){
        memset(board, 0, sizeof(int) * SQUARE_AMOUNT * SQUARE_AMOUNT);
        *occupiedCells = 0;
        initializeNewValue(board, 3, occupiedCells, newValueX, newValueY, biggestTile);
        *newValueX = -1;
        *newValueY = -1;
        *currentScore = 0;
        return 1;
    }
    else
        return -1;
}

int updateDisplay(int *board, int highScore, int currentScore, int newValueX, int newValueY, int badInput, int biggestTile){
    if(board != NULL){
        if(drawBoard(board, newValueX, newValueY, currentScore, badInput, highScore, biggestTile) == -1)
            return -1;
        printf("%10s - %-1c    %15s - %-1c\n", CONTROL_UP_MSG, UP, CONTROL_DOWN_MSG, DOWN);
        printf("%10s - %-1c    %15s - %-1c\n", CONTROL_LEFT_MSG, LEFT, CONTROL_RIGHT_MSG, RIGHT);
        printf("%10s - %-1c    %15s - %-1c\n", CONTROL_SAVE_GAME_MSG, SAVE_GAME, CONTROL_NEW_GAME_MSG, START_NEW_GAME);
        return 1;
    }
    else
        return -1;
}

void endGameError(int *board){
    free(board);
    printf(GAME_ERROR_MSG);
    exit(0);
}


int main(){
    int *board = (int *)calloc(SQUARE_AMOUNT * SQUARE_AMOUNT, sizeof(int));
    if(board == NULL){
        endGameError(board);
    }

    int occupiedCells = 0; //counter to keep track of occupied cells
    int currentScore = 0; //current game score
    int highScore = 0; //all time highest score
    attempts = 1; //keep track of game attempts
    int biggestTile = 0; //keep track of the biggest tile ever seen
    start = clock(); //start time measuring
    int initializeNewValues = 1; //check if needed to initialize initial cell values
    int newValueX, newValueY; //store cell to highlight
    char move = ' '; //store player's move
    int badInput = 0; //check if made moves are correct

    atexit(timeSpent); //log time spent playing
    srand(time(NULL)); //update seed for random values

    if(readFromFile(board, &occupiedCells, &currentScore, &highScore, &initializeNewValues, &biggestTile)){
        printf(FOUND_PROGRESS_MSG);
        ++attempts;
    }
    else
        printf(NEW_GAME_MSG);

    if(occupiedCells < SQUARE_AMOUNT * SQUARE_AMOUNT && initializeNewValues){
        if(initializeNewValue(board, 3, &occupiedCells, &newValueX, &newValueY, &biggestTile) == -1){
            endGameError(board);
        }
    }

    newValueX = -1, newValueY = -1; //not to highlight initial values

    if(updateDisplay(board, highScore, currentScore, newValueX, newValueY, badInput, biggestTile) == -1){
        endGameError(board);
    }

    while (1) {
        if (occupiedCells == SQUARE_AMOUNT * SQUARE_AMOUNT && endGame(board)) {
            printf(GAME_END_MSG);
            highScore = currentScore > highScore ? currentScore : highScore;
            if(startNewGame(board, &occupiedCells, &currentScore, &newValueX, &newValueY, &biggestTile) == -1){
                endGameError(board);
            }
            if(saveProgress(board, occupiedCells, currentScore, highScore, biggestTile) == -1){
                endGameError(board);
            }
            free(board);
            break;
        }

        if (_kbhit()) {
            move = toupper(getch());
            if (move == UP || move == DOWN || move == RIGHT || move == LEFT || move == SAVE_GAME || move == START_NEW_GAME) {
                badInput = 0; //update value if correct move recorded

                if (move == UP) {
                    if(moveVertically(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1){
                        endGameError(board);
                    }
                }

                if (move == DOWN) {
                    if(moveVertically(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1){
                        endGameError(board);
                    }
                }

                if (move == LEFT) {
                    if(moveHorizontally(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1){
                        endGameError(board);
                    }
                }

                if (move == RIGHT) {
                    if(moveHorizontally(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1){
                        endGameError(board);
                    }
                }

                if (move == SAVE_GAME) { // save and exit
                    if(saveProgress(board, occupiedCells, currentScore, highScore, biggestTile) == -1){
                        endGameError(board);
                    }
                    free(board);
                    break;
                }

                if (move == START_NEW_GAME) { // new game
                    if(startNewGame(board, &occupiedCells, &currentScore, &newValueX, &newValueY, &biggestTile) == -1){
                        endGameError(board);
                    }
                }
            } else {
                system("cls"); // clears terminal
                badInput = 1; // prints bad input message
                if(updateDisplay(board, highScore, currentScore, newValueX, newValueY, badInput, biggestTile) == -1){
                    endGameError(board);
                }
            }
            //save progress in case user shuts down program unexpectedly
            if(saveProgress(board, occupiedCells, currentScore, highScore, biggestTile) == -1){
                endGameError(board);
            }
            system("cls"); // clears terminal
            highScore = highScore > currentScore ? highScore : currentScore; //keep track of high score
            // update information shown on screen
            if (updateDisplay(board, highScore, currentScore, newValueX, newValueY, badInput, biggestTile) == -1){
                endGameError(board);
            }
        }
    }
    return 0;
}
