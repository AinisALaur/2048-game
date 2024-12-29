// AUTHOR: Ainis Augustas Laurinavicius
// DATE: 2024/12/29
// DESCRIPTION: 2048 GAME'S MAIN PROGRAM FILE

#include <stdio.h>
#include <stdlib.h> // READ AND WRITE FILES
#include <time.h>   // TO GET RANDOM VALUES FOR THE BOARD
#include <string.h> // FOR MEMORY FUNCTIONS
#include <windows.h> // TO HANDLE UNEXPECTED PROGRAM SHUTDOWN
#include <conio.h>   // TRACK KEYBOARD EVENTS
#include <ctype.h>   // FOR FUNCTION TOUPPER()
#include "2048.h"  // HEADER FILE

// BASIC SETTINGS
#define SQUARE_SIZE 11 // TESTED WITH 11
#define VALUE_COLOR  "\x1B[36m" // CYAN
#define ACHIEVEMENT_COLOR  "\x1B[33m" // YELLOW
#define HIGHLIGHT_COLOR  "\x1B[32m" // GREEN
#define BAD_INPUT_COLOR  "\x1B[31m" // RED
#define COLOR_RESET  "\x1B[37m" // WHITE
#define NUM_POSSIBLE_CELL_VALUES 10
#define POSSIBLE_CELL_VALUES {4, 2, 2, 2, 2, 2, 2, 2, 2, 2} // 4 HAS A 10% TO APPEAR

// PRINTF MESSAGES
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

// CONTROLS FOR THE GAME
#define UP 'W'
#define DOWN 'S'
#define LEFT 'A'
#define RIGHT 'D'
#define SAVE_GAME 'E'
#define START_NEW_GAME 'N'

// GLOBAL VARIABLES TO MAKE IT WORK WITH ATEXIT()
clock_t start;
int attempts;

// GET NUMBER SIZE TO ALIGN TO CENTER WHEN PRINTING
int getNumSize(int value) {
    int t = 0;
    while (value > 0) {
        t++;
        value /= 10;
    }
    return t;
}

// CHECK IF NO MORE AVAILABLE MOVES
int endGame(int *board) {
    if (board != NULL) {
        for (int i = 0; i < SQUARE_AMOUNT; ++i) {
            for (int x = 0; x < SQUARE_AMOUNT; ++x) {
                int current = *(board + SQUARE_AMOUNT * i + x);

                // CHECK LEFT NEIGHBOR
                if (x > 0 && current == *(board + SQUARE_AMOUNT * i + (x - 1))) {
                    return 0;
                }

                // CHECK RIGHT NEIGHBOR
                if (x < SQUARE_AMOUNT - 1 && current == *(board + SQUARE_AMOUNT * i + (x + 1))) {
                    return 0;
                }

                // CHECK TOP NEIGHBOR
                if (i > 0 && current == *(board + SQUARE_AMOUNT * (i - 1) + x)) {
                    return 0;
                }

                // CHECK BOTTOM NEIGHBOR
                if (i < SQUARE_AMOUNT - 1 && current == *(board + SQUARE_AMOUNT * (i + 1) + x)) {
                    return 0;
                }
            }
        }
        return 1;
    }
    return -1;
}

// PRINT A STRAIGHT HORIZONTAL LINE FOR BOARD VISUALIZATION
void drawLine(int length) {
    for (int i = 0; i < SQUARE_AMOUNT; ++i) {
        for (int x = 0; x < length; ++x) {
            printf("-", x);
        }
    }
    printf("-\n");
}

// DRAW GAME BOARD
int drawBoard(int *board, int newValueX, int newValueY, int currentScore, int badInput, int highScore, int biggestTile) {
    if (board != NULL) {
        for (int j = 0; j < SQUARE_AMOUNT; ++j) {
            drawLine(SQUARE_SIZE);

            for (int i = 0; i < SQUARE_SIZE / 2; ++i) {
                for (int x = 0; x < SQUARE_AMOUNT; ++x) {
                    if (i == SQUARE_SIZE / 4 && *(board + SQUARE_AMOUNT * j + x) != 0) {

                        // CALCULATE PADDING TO ALIGN NUMBER TO CENTER
                        int cellWidth = SQUARE_SIZE - 1;
                        int sizeOfNum = getNumSize(*(board + SQUARE_AMOUNT * j + x));
                        int leftPadding = (cellWidth - sizeOfNum) / 2;
                        int rightPadding = cellWidth - leftPadding - sizeOfNum;

                        if (cellWidth < sizeOfNum) {
                            return -1; // IF TEXT OVERFLOW OCCURS
                        }

                        char cellValueColor[] = VALUE_COLOR;
                        if (j == newValueY && x == newValueX)
                            strcpy(cellValueColor, HIGHLIGHT_COLOR);

                        printf("|%s%*s%d%s%*s", cellValueColor, leftPadding, "", *(board + SQUARE_AMOUNT * j + x), COLOR_RESET, rightPadding, "");
                    } else {
                        printf("|%*s", SQUARE_SIZE - 1, "");
                    }
                }

                Achievement *Achievements = (Achievement *)malloc(4 * sizeof(Achievement));
                if (Achievements == NULL) {
                    return -1; // IF MEMORY ALLOCATION FAILED
                }

                Achievements = sortByValues(attempts, highScore, currentScore, biggestTile); // SORT ACHIEVEMENTS IN DESCENDING ORDER
                if (Achievements == NULL) { // CHECK IF MEMORY ALLOCATION WAS SUCCESSFUL INSIDE FUNCTION
                    return -1;
                }

                if (j == 0 && i == 0)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[0].name, Achievements[0].value, COLOR_RESET);
                else if (j == 0 && i == 1)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[1].name, Achievements[1].value, COLOR_RESET);
                else if (j == 0 && i == 2)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[2].name, Achievements[2].value, COLOR_RESET);
                else if (j == 0 && i == 3)
                    printf("|  %s%s%d%s\n", ACHIEVEMENT_COLOR, Achievements[3].name, Achievements[3].value, COLOR_RESET);
                else if (j == 2 && i == 1 && badInput)
                    printf("|  %s%s%s\n", BAD_INPUT_COLOR, BAD_INPUT_MSG, COLOR_RESET);
                else
                    printf("|\n");

                free(Achievements);
            }
        }

        drawLine(SQUARE_SIZE);
        return 1;
    } else {
        return -1;
    }
}

// ADDS INITIAL VALUES AND EXTRA VALUE AFTER A VALID MOVE
int initializeNewValue(int *board, int NumberOfValues, int *occupiedCells, int *newValueX, int *newValueY, int *biggestTile) {
    if (board != NULL && newValueX != NULL && newValueY != NULL && newValueX != NULL && biggestTile != NULL) {
        int x, y;
        for (int i = 0; i < NumberOfValues; ++i) {
            int IsOccupied = 1;
            while (IsOccupied) {
                x = rand() % SQUARE_AMOUNT;
                y = rand() % SQUARE_AMOUNT;

                if (*(board + SQUARE_AMOUNT * y + x) == 0)
                    IsOccupied = 0;
            }

            int newCellValue[NUM_POSSIBLE_CELL_VALUES] = POSSIBLE_CELL_VALUES;
            int newIndex = rand() % 10;
            *(board + SQUARE_AMOUNT * y + x) = newCellValue[newIndex];
            *biggestTile = (*biggestTile) > newCellValue[newIndex] ? *biggestTile : newCellValue[newIndex];
            (*occupiedCells) += 1;
            *newValueX = x;
            *newValueY = y;
        }
        return 1;
    } else {
        return -1;
    }
}

// MOVE ALL CELLS VERTICALLY
int moveAllCellsVertically(int *board, int *occupiedCells, char direction) {
    if (board != NULL && occupiedCells != NULL) {
        // ASSIGN APPROPRIATE Y VALUE FOR THE UPCOMING LOOP
        int y = direction == UP ? 1 : SQUARE_AMOUNT - 2;

        // MOVE ALL ELEMENTS UP/DOWN
        while (1) { // START AT SECOND ROW FROM TOP AND PROGRESS DOWN OR SECOND ROW FROM BOTTOM
            // BREAK FUNCTION WHEN NEEDED
            if ((direction == UP && y > SQUARE_AMOUNT - 1) || (direction == DOWN && y < 0)) {
                break;
            }

            for (int x = 0; x < SQUARE_AMOUNT; ++x) { // GO THROUGH ALL ROW'S ELEMENTS
                int cellValue = *(board + SQUARE_AMOUNT * y + x);
                if (cellValue != 0) { // IF ELEMENT IS NOT ZERO, MOVE IT AS HIGH/LOW AS POSSIBLE
                    *(board + SQUARE_AMOUNT * y + x) = 0;

                    int ValuesYcord;
                    if (direction == UP) {
                        ValuesYcord = 0;
                        while (*(board + SQUARE_AMOUNT * ValuesYcord + x) != 0) // INCREASING ITS Y COORDINATE TO THE MAXIMUM
                            ValuesYcord++;
                    } else {
                        ValuesYcord = SQUARE_AMOUNT - 1;
                        while (*(board + SQUARE_AMOUNT * ValuesYcord + x) != 0) // DECREASING ITS Y COORDINATE TO THE MAXIMUM
                            ValuesYcord--;
                    }

                    *(board + SQUARE_AMOUNT * ValuesYcord + x) = cellValue;
                }
            }

            // MOVE UP A ROW OR DOWN
            direction == UP ? ++y : --y;
        }
        return 1;
    } else {
        return -1;
    }
}


int moveVertically(int *board, int *occupiedCells, char direction, int *newValueX, int *newValueY, int *currentScore, int *biggestTile) {
    if (board != NULL && occupiedCells != NULL && newValueX != NULL && newValueY != NULL && currentScore != NULL && biggestTile != NULL) {
        int *movedCells = (int *)calloc(SQUARE_AMOUNT * SQUARE_AMOUNT, sizeof(int));
        if (movedCells == NULL) {
            free(movedCells);
            return -1;
        }

        int y = direction == UP ? SQUARE_AMOUNT - 1 : 0;

        int *initialBoard = (int *)malloc(SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));
        if (initialBoard == NULL) {
            free(initialBoard);
            return -1;
        }
        memcpy(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));

        while (1) { // STARTS AT BOTTOM ROW AND PROGRESSES UP
            // CHECK WHEN TO STOP CYCLE
            if (direction == UP && y <= 0 || direction == DOWN && y >= SQUARE_AMOUNT - 1) {
                break;
            }

            for (int x = 0; x < SQUARE_AMOUNT; ++x) { // GOES THROUGH ROW'S ELEMENTS
                if (*(board + SQUARE_AMOUNT * y + x) != 0) { // IF ELEMENT IS NOT 0, LOOK IF ANY NON-ZERO ELEMENTS EXIST ABOVE/BELOW

                    // ASSIGN J ACCORDINGLY
                    int y1 = direction == UP ? y - 1 : y + 1;

                    while (1) { // LOOP THAT CHECKS ALL ELEMENTS ABOVE/BELOW
                        if (direction == UP && y1 < 0 || direction == DOWN && y1 > SQUARE_AMOUNT - 1) {
                            break;
                        }

                        // IF FINDS THE SAME ELEMENT ABOVE IT
                        if (*(board + SQUARE_AMOUNT * y1 + x) != 0 && *(board + SQUARE_AMOUNT * y1 + x) == *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0) {
                            *currentScore += (*(board + SQUARE_AMOUNT * y1 + x)) * 2;
                            *(board + SQUARE_AMOUNT * y1 + x) = (*(board + SQUARE_AMOUNT * y1 + x)) * 2;
                            *(board + SQUARE_AMOUNT * y + x) = 0;
                            *biggestTile = (*biggestTile) > *(board + SQUARE_AMOUNT * y1 + x) ? *biggestTile : *(board + SQUARE_AMOUNT * y1 + x);
                            (*occupiedCells)--;
                            *(movedCells + SQUARE_AMOUNT * y1 + x) = 1; // MAKE SURE TO NOT MOVE ALREADY MOVED VALUES
                            break;
                        }

                        // IF FINDS A NON-ZERO ELEMENT THAT IS NOT EQUAL TO ORIGINAL
                        else if (*(board + SQUARE_AMOUNT * y1 + x) != 0 && *(board + SQUARE_AMOUNT * y1 + x) != *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0) {
                            int new_value = *(board + SQUARE_AMOUNT * y + x);
                            *(board + SQUARE_AMOUNT * y + x) = 0;
                            int Yvalue = direction == UP ? y1 + 1 : y1 - 1;

                            *(board + SQUARE_AMOUNT * Yvalue + x) = new_value;
                            *(movedCells + SQUARE_AMOUNT * Yvalue + x) = 1; // MAKE SURE TO NOT MOVE ALREADY MOVED VALUES
                            break;
                        }

                        // MOVE TO UPPER OR LOWER ROW
                        direction == UP ? --y1 : ++y1;
                    }
                }
            }
            // MOVE UP OR DOWN
            direction == UP ? --y : ++y;
        }

        if (moveAllCellsVertically(board, occupiedCells, direction) == -1)
            return -1;

        if (memcmp(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int)) != 0)
            initializeNewValue(board, 1, occupiedCells, newValueX, newValueY, biggestTile); // ADD NEW VALUE TO BOARD
        free(movedCells);
        free(initialBoard);
        return 1;
    } else
        return -1;
}

int moveAllCellsHorizontally(int *board, int *occupiedCells, char direction) {
    if (board != NULL && occupiedCells != NULL) {
        int x = direction == LEFT ? 1 : SQUARE_AMOUNT - 2;
        while (1) {
            if (direction == LEFT && x > SQUARE_AMOUNT - 1 || direction == RIGHT && x < 0)
                break;

            for (int y = 0; y < SQUARE_AMOUNT; ++y) {
                if (*(board + SQUARE_AMOUNT * y + x) != 0) {
                    int value = *(board + SQUARE_AMOUNT * y + x);
                    *(board + SQUARE_AMOUNT * y + x) = 0;

                    int xcor;

                    if (direction == LEFT) {
                        xcor = 0;
                        // DECREASE X COORDINATE TO THE MAXIMUM (MOVE LEFT)
                        while (*(board + SQUARE_AMOUNT * y + xcor) != 0 && xcor < SQUARE_AMOUNT - 1) {
                            xcor++;
                        }
                    }

                    if (direction == RIGHT) {
                        xcor = SQUARE_AMOUNT - 1;
                        // INCREASE X COORDINATE TO THE MAXIMUM (MOVE RIGHT)
                        while (*(board + SQUARE_AMOUNT * y + xcor) != 0 && xcor > 0) {
                            xcor--;
                        }
                    }

                    *((int *)board + SQUARE_AMOUNT * y + xcor) = value;
                }
            }
            direction == LEFT ? ++x : --x;
        }
        return 1;
    } else
        return -1;
}

int moveHorizontally(int *board, int *occupiedCells, char direction, int *newValueX, int *newValueY, int *currentScore, int *biggestTile) {
    if (board != NULL && occupiedCells != NULL && newValueX != NULL && newValueY != NULL && currentScore != NULL && biggestTile != NULL) {
        int *movedCells = (int *)calloc(SQUARE_AMOUNT * SQUARE_AMOUNT, sizeof(int));
        if (movedCells == NULL) {
            free(movedCells);
            return -1;
        }

        int x = direction == LEFT ? SQUARE_AMOUNT - 1 : 0;
        int *initialBoard = (int *)malloc(SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));
        if (initialBoard == NULL) {
            free(initialBoard);
            return -1;
        }
        memcpy(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int));

        // MOVE LEFT OR RIGHT
        while (1) {
            if (direction == LEFT && x < 1 || direction == RIGHT && x > SQUARE_AMOUNT - 1)
                break;

            for (int y = 0; y < SQUARE_AMOUNT; ++y) {
                if (*(board + SQUARE_AMOUNT * y + x) != 0) {
                    int x1 = direction == LEFT ? x - 1 : x + 1;
                    while (1) {
                        if (direction == LEFT && x1 < 0 || direction == RIGHT && x1 > SQUARE_AMOUNT - 1)
                            break;

                        if (*((int *)board + SQUARE_AMOUNT * y + x1) != 0 && *(board + SQUARE_AMOUNT * y + x1) == *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0) {
                            *currentScore += (*(board + SQUARE_AMOUNT * y + x)) * 2;
                            *(board + SQUARE_AMOUNT * y + x1) = (*(board + SQUARE_AMOUNT * y + x)) * 2;
                            *biggestTile = (*biggestTile) > *(board + SQUARE_AMOUNT * y + x1) ? *biggestTile : *(board + SQUARE_AMOUNT * y + x1);
                            *(board + SQUARE_AMOUNT * y + x) = 0;
                            (*occupiedCells)--;
                            *(movedCells + SQUARE_AMOUNT * y + x1) = 1;
                            break;
                        }

                        if (*(board + SQUARE_AMOUNT * y + x1) != 0 && *(board + SQUARE_AMOUNT * y + x1) != *(board + SQUARE_AMOUNT * y + x) && *(movedCells + SQUARE_AMOUNT * y + x) == 0) {
                            int value = *(board + 4 * y + x);
                            *(board + SQUARE_AMOUNT * y + x) = 0;
                            int Xvalue = direction == LEFT ? x1 + 1 : x1 - 1;

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

        if (moveAllCellsHorizontally(board, occupiedCells, direction) == -1)
            return -1;

        if (memcmp(initialBoard, board, SQUARE_AMOUNT * SQUARE_AMOUNT * sizeof(int)) != 0)
            initializeNewValue(board, 1, occupiedCells, newValueX, newValueY, biggestTile); // ADD NEW VALUE TO BOARD
        free(movedCells);
        free(initialBoard);
        return 1;
    } else
        return -1;
}

int startNewGame(int *board, int *occupiedCells, int *currentScore, int *newValueX, int *newValueY, int *biggestTile) {
    if (board != NULL && occupiedCells != NULL && currentScore != NULL && newValueX != NULL && newValueY != NULL && biggestTile != NULL) {
        memset(board, 0, sizeof(int) * SQUARE_AMOUNT * SQUARE_AMOUNT);
        *occupiedCells = 0;
        initializeNewValue(board, 3, occupiedCells, newValueX, newValueY, biggestTile);
        *newValueX = -1;
        *newValueY = -1;
        *currentScore = 0;
        return 1;
    } else
        return -1;
}

int updateDisplay(int *board, int highScore, int currentScore, int newValueX, int newValueY, int badInput, int biggestTile) {
    if (board != NULL) {
        if (drawBoard(board, newValueX, newValueY, currentScore, badInput, highScore, biggestTile) == -1)
            return -1;
        printf("%10s - %-1c    %15s - %-1c\n", CONTROL_UP_MSG, UP, CONTROL_DOWN_MSG, DOWN);
        printf("%10s - %-1c    %15s - %-1c\n", CONTROL_LEFT_MSG, LEFT, CONTROL_RIGHT_MSG, RIGHT);
        printf("%10s - %-1c    %15s - %-1c\n", CONTROL_SAVE_GAME_MSG, SAVE_GAME, CONTROL_NEW_GAME_MSG, START_NEW_GAME);
        return 1;
    } else
        return -1;
}

void endGameError(int *board) {
    free(board);
    printf(GAME_ERROR_MSG);
    exit(0);
}

int main() {
    int *board = (int *)calloc(SQUARE_AMOUNT * SQUARE_AMOUNT, sizeof(int));
    if (board == NULL) {
        endGameError(board);
    }

    int occupiedCells = 0; // COUNTER TO KEEP TRACK OF OCCUPIED CELLS
    int currentScore = 0; // CURRENT GAME SCORE
    int highScore = 0; // ALL TIME HIGHEST SCORE
    attempts = 1; // KEEP TRACK OF GAME ATTEMPTS
    int biggestTile = 0; // KEEP TRACK OF THE BIGGEST TILE EVER SEEN
    start = clock(); // START TIME MEASURING
    int initializeNewValues = 1; // CHECK IF NEEDED TO INITIALIZE INITIAL CELL VALUES
    int newValueX, newValueY; // STORE CELL TO HIGHLIGHT
    char move = ' '; // STORE PLAYER'S MOVE
    int badInput = 0; // CHECK IF MADE MOVES ARE CORRECT

    atexit(timeSpent); // LOG TIME SPENT PLAYING
    srand(time(NULL)); // UPDATE SEED FOR RANDOM VALUES

    if (readFromFile(board, &occupiedCells, &currentScore, &highScore, &biggestTile)) {
        printf(FOUND_PROGRESS_MSG);
        initializeNewValues = 0;
        ++attempts;
    } else {
        printf(NEW_GAME_MSG);
    }

    if (occupiedCells < SQUARE_AMOUNT * SQUARE_AMOUNT && initializeNewValues) {
        if (initializeNewValue(board, 3, &occupiedCells, &newValueX, &newValueY, &biggestTile) == -1) {
            endGameError(board);
        }
    }

    newValueX = -1, newValueY = -1; // NOT TO HIGHLIGHT INITIAL VALUES

    if (updateDisplay(board, highScore, currentScore, newValueX, newValueY, badInput, biggestTile) == -1) {
        endGameError(board);
    }

    while (1) {
        if (occupiedCells == SQUARE_AMOUNT * SQUARE_AMOUNT && endGame(board)) {
            printf(GAME_END_MSG);
            highScore = currentScore > highScore ? currentScore : highScore;
            if (startNewGame(board, &occupiedCells, &currentScore, &newValueX, &newValueY, &biggestTile) == -1) {
                endGameError(board);
            }
            if (saveProgress(board, occupiedCells, currentScore, highScore, biggestTile) == -1) {
                endGameError(board);
            }
            free(board);
            break;
        }

        if (_kbhit()) {
            move = toupper(getch());
            if (move == UP || move == DOWN || move == RIGHT || move == LEFT || move == SAVE_GAME || move == START_NEW_GAME) {
                badInput = 0; // UPDATE VALUE IF CORRECT MOVE RECORDED

                if (move == UP) {
                    if (moveVertically(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1) {
                        endGameError(board);
                    }
                }

                if (move == DOWN) {
                    if (moveVertically(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1) {
                        endGameError(board);
                    }
                }

                if (move == LEFT) {
                    if (moveHorizontally(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1) {
                        endGameError(board);
                    }
                }

                if (move == RIGHT) {
                    if (moveHorizontally(board, &occupiedCells, move, &newValueX, &newValueY, &currentScore, &biggestTile) == -1) {
                        endGameError(board);
                    }
                }

                if (move == SAVE_GAME) { // SAVE AND EXIT
                    if (saveProgress(board, occupiedCells, currentScore, highScore, biggestTile) == -1) {
                        endGameError(board);
                    }
                    free(board);
                    break;
                }

                if (move == START_NEW_GAME) { // NEW GAME
                    if (startNewGame(board, &occupiedCells, &currentScore, &newValueX, &newValueY, &biggestTile) == -1) {
                        endGameError(board);
                    }
                }
            } else {
                system("cls"); // CLEARS TERMINAL
                badInput = 1; // PRINTS BAD INPUT MESSAGE
                if (updateDisplay(board, highScore, currentScore, newValueX, newValueY, badInput, biggestTile) == -1) {
                    endGameError(board);
                }
            }
            // SAVE PROGRESS IN CASE USER SHUTS DOWN PROGRAM UNEXPECTEDLY
            if (saveProgress(board, occupiedCells, currentScore, highScore, biggestTile) == -1) {
                endGameError(board);
            }
            system("cls"); // CLEARS TERMINAL
            highScore = highScore > currentScore ? highScore : currentScore; // KEEP TRACK OF HIGH SCORE
            // UPDATE INFORMATION SHOWN ON SCREEN
            if (updateDisplay(board, highScore, currentScore, newValueX, newValueY, badInput, biggestTile) == -1) {
                endGameError(board);
            }
        }
    }
    return 0;
}
