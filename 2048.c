//Author: Ainis Augustas Laurinavicius
//Date: 2024/12/26

#include<stdio.h>
#include<stdlib.h> // read and write files
#include<time.h> //to get random values for board
#include<string.h> //for mem functions
#include <windows.h> //to handle unexpected program shutdown

#define SQUARESIZE 11 //tested with 11

#define COLOR  "\x1B[36m" //cyan
#define SCORECOLOR  "\x1B[33m" //yellow
#define HIGHLIGHTCOLOR  "\x1B[32m" //green
#define BADINPUTCOLOR  "\x1B[31m" //reds
#define COLOR_RESET  "\x1B[37m" //whites

#define SQUAREAMOUNT 4 //size of board 4x4 tested with 4

#define FILENAME "Progress.dat" //name of file where to save progress

//PRINTF messages
#define FOUNDPROGRESS "Game progress found!"
#define NEWGAME "Starting a new game!"
#define GAMENAME "2048 GAME"
#define AUTHOR "MADE AND DESIGNED BY AINIS AUGUSTAS LAURINAVICIUS"
#define TEXTOVERFLOW "THE VALUES DO NOT FIT IN THE CELLS, PLEASE SELECT A BIGGER CELL SIZE!!"
#define ENDMSG "Game has ended"
#define CONTROLS "u - up     e - save progress and exit\nd - down   n - new game\nl - left\nr - right\nEnter your move: "
#define SCOREMSG "SCORE: "
#define HIGHSCOREMSG "HIGH SCORE: "
#define BADINPUTMSG "BAD INPUT"

//CONTROLS FOR THE GAME
#define UP 'W'
#define DOWN 'S'
#define LEFT 'A'
#define RIGHT 'D'

//make global variables so when program shuts down unexpectedly - saves progress correctly
int board[SQUAREAMOUNT][SQUAREAMOUNT] = { };
int occupiedCells = 0;
int current_score = 0;
int high_score = 0;

//save progress to file
void saveProgress(){
    FILE* progressFile = fopen(FILENAME, "wb");
    if (progressFile != NULL) {
        fwrite(&board, sizeof(int), SQUAREAMOUNT * SQUAREAMOUNT, progressFile);
        fwrite(&occupiedCells, sizeof(int), 1, progressFile);
        fwrite(&current_score, sizeof(int), 1, progressFile);
        fwrite(&high_score, sizeof(int), 1, progressFile);
        fclose(progressFile);
    }
}

//handle when program shuts down unexpectedly
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT) {
        saveProgress();
    }
    return TRUE;
}

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
int gameEnds(int *board) {
    for (int i = 0; i < SQUAREAMOUNT; ++i) {
        for (int x = 0; x < SQUAREAMOUNT; ++x) {
            int current = *(board + SQUAREAMOUNT * i + x);

            // Check left neighbor
            if (x > 0 && current == *(board + SQUAREAMOUNT * i + (x - 1))) {
                return 0;
            }

            // Check right neighbor
            if (x < SQUAREAMOUNT-1 && current == *(board + SQUAREAMOUNT * i + (x + 1))) {
                return 0;
            }

            // Check top neighbor
            if (i > 0 && current == *(board + SQUAREAMOUNT * (i - 1) + x)) {
                return 0;
            }

            // Check bottom neighbor
            if (i < SQUAREAMOUNT-1 && current == *(board + SQUAREAMOUNT * (i + 1) + x)) {
                return 0;
            }
        }
    }

    return 1;
}

// print a straight horizontal line for board visualization
void straightLine(int length){
    for(int i =0; i < SQUAREAMOUNT; ++i){
        for(int x=0; x < length; ++x){
            printf("-", x);
        }
    }printf("-\n");
}


//draw game board
void drawBoard(int *board, int newValueX, int newValueY, int current_score, int badInput, int high_score){
    for(int j = 0; j < SQUAREAMOUNT; ++j){
        straightLine(SQUARESIZE);
        for(int i = 0; i < SQUARESIZE/2; ++i){
            for(int x = 0; x < SQUAREAMOUNT; ++x){
                if(i == SQUARESIZE / 4 && *(board + SQUAREAMOUNT * j + x) != 0){

                    //calculate padding to align number to center
                    int cellWidth = SQUARESIZE - 1;
                    int sizeOfNum = getNumSize(*(board + SQUAREAMOUNT * j + x));
                    int leftPadding = (cellWidth - sizeOfNum) / 2;
                    int rightPadding = cellWidth - leftPadding - sizeOfNum;


                    if(cellWidth < sizeOfNum){
                        printf("%s", TEXTOVERFLOW);
                        exit(0);
                    }


                    char color[] = COLOR;
                    if(j == newValueY && x == newValueX)
                        strcpy(color,HIGHLIGHTCOLOR);

                    printf("|%s%*s%d%s%*s", color,leftPadding,"",*(board + SQUAREAMOUNT * j + x),COLOR_RESET,rightPadding,"");
                }
                else{
                    printf("|%*s",SQUARESIZE - 1,"");
                }
            }

            if(j == 0 && i == 0)
                printf("|  %s\n", GAMENAME);
            else if(j == 0 && i == 1)
                printf("|  %s\n", AUTHOR);
            else if(j == 1 && i == 0)
                printf("|  %s%s%d%s\n", SCORECOLOR, SCOREMSG, current_score, COLOR_RESET);
            else if(j == 1 && i == 1)
                printf("|  %s%s%d%s\n", SCORECOLOR, HIGHSCOREMSG, high_score > current_score? high_score : current_score, COLOR_RESET);
            else if(j == 2 && i == 1 && badInput)
                printf("|  %s%s%s\n", BADINPUTCOLOR, BADINPUTMSG, COLOR_RESET);
            else
                printf("|\n");
        }
    }
    straightLine(SQUARESIZE);
}

//Adds initial values and extra value after a valid move
int initializeNewValue(int *board, int NumberOfValues, int *occupiedCells, int *NewValueX, int *NewValueY){
    int x, y;
    for(int i = 0; i < NumberOfValues; ++i){
        int IsOccupied = 1;
        while(IsOccupied){
            x = rand() % SQUAREAMOUNT;
            y = rand() % SQUAREAMOUNT;

            if(*(board + SQUAREAMOUNT * y + x) == 0)
                IsOccupied = 0;
        }
        int newCellValue[10] = {4,2,2,2,2,2,2,2,2,2}; // 4 has a 10% to appear
        int newIndex = rand() % 10;
        *(board + SQUAREAMOUNT * y + x) = newCellValue[newIndex];
        (*occupiedCells) += 1;
        *NewValueX = x;
        *NewValueY = y;
    }
}

void boardMovesVertically(int *board, int *occupiedCells, char direction, int *NewValueX, int *NewValueY, int *current_score){
    int moved_cells[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    int y = direction == UP? SQUAREAMOUNT-1: 0;

    int initialBoard[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    memcpy(initialBoard, board, SQUAREAMOUNT * SQUAREAMOUNT *sizeof(int));

    while(1){ //starts at bottom row and progresses up

        //check when to stop cycle
        if(direction == UP && y <= 0 || direction == DOWN && y >= SQUAREAMOUNT - 1){
            break;
        }

        for(int x = 0; x < SQUAREAMOUNT; ++x){ //goes trough row's elements
            if(*(board + SQUAREAMOUNT * y + x) !=0 ){ // if element is not 0 we look if any non 0 elements exist above/below

                //assign j accordingly
                int  y1 = direction == UP? y - 1: y + 1;

                while(1){ // loop that checks all elements above/below
                    if(direction == UP && y1 < 0 || direction == DOWN && y1 > SQUAREAMOUNT - 1){
                        break;
                    }

                    //if finds the same element above it
                    if(*(board + SQUAREAMOUNT * y1 + x) != 0 && *(board + SQUAREAMOUNT * y1 + x) == *(board + SQUAREAMOUNT * y + x) && moved_cells[y][x] == 0){
                       *current_score += (*(board + SQUAREAMOUNT * y1 + x)) * 2;
                       *(board+SQUAREAMOUNT* y1 + x) = (*(board + SQUAREAMOUNT * y1 + x)) * 2;
                       *(board+SQUAREAMOUNT* y + x) = 0;
                       (*occupiedCells)--;
                        moved_cells[y1][x] = 1; // make sure to not move already moved values
                        break;
                    }

                    //if finds a non 0 element that is not equal to original
                    else if(*(board + SQUAREAMOUNT * y1 + x) != 0 && *(board + SQUAREAMOUNT * y1 + x) != *(board + SQUAREAMOUNT * y + x) && moved_cells[y][x] == 0){
                       int new_value = *(board + SQUAREAMOUNT * y + x);
                       *(board + SQUAREAMOUNT * y + x) = 0;
                       int Yvalue = direction == UP? y1 + 1: y1 - 1;

                       *(board+SQUAREAMOUNT*Yvalue+x) = new_value;
                       moved_cells[Yvalue][x] = 1; // make sure to not move already moved values
                       break;
                    }

                    //move to upper or lower row
                    direction == UP? --y1: ++y1;
                }
            }
        }

        //move up or down
        direction == UP? --y: ++y;
    }

    //assign appropriate i value for upcoming loop
    y=direction == UP? 1: SQUAREAMOUNT - 2;

    //Move all elements up/down
    while(1){ // start at second row from top and progress down or second row from bottom

        //break function when needed
        if(direction == UP && y > SQUAREAMOUNT - 1 || direction == DOWN && y < 0){
            break;
        }

        for(int x = 0; x < SQUAREAMOUNT; ++x){ // go through all row's elements
            int cellValue = *(board + SQUAREAMOUNT * y + x);
            if(cellValue != 0){ // if element is not zero move it as high/low up as possible
                *(board + SQUAREAMOUNT * y + x) = 0;
                int ValuesYcord;
                if(direction == UP){
                    ValuesYcord = 0;
                    while(*(board + SQUAREAMOUNT * ValuesYcord + x) != 0) // increasing it's y coordinate to the maximum
                        ValuesYcord++;
                }
                else{
                    ValuesYcord = SQUAREAMOUNT - 1;
                    while(*(board + SQUAREAMOUNT * ValuesYcord + x) != 0) // decreasing it's y coordinate to the maximum
                        ValuesYcord--;
                }

                *(board + SQUAREAMOUNT * ValuesYcord + x) = cellValue;
            }
        }
       // move up a row or down
       direction == UP? ++y: --y;
    }

    if(memcmp(initialBoard, board, SQUAREAMOUNT * SQUAREAMOUNT * sizeof(int)) != 0)
        initializeNewValue(board, 1, occupiedCells, NewValueX, NewValueY); // add new value to board
}

void boardMovesHorizontally(int *board, int *occupiedCells, char direction, int *NewValueX, int *NewValueY, int *current_score){
    int moved_cells[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    int x = direction == LEFT? SQUAREAMOUNT-1: 0;

    int initialBoard[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    memcpy(initialBoard, board, SQUAREAMOUNT * SQUAREAMOUNT * sizeof(int));


    //move left or right
    while(1){

        if(direction == LEFT && x < 1 || direction == RIGHT && x > SQUAREAMOUNT - 1)
            break;

        for(int y = 0; y < SQUAREAMOUNT; ++y){
            if(*(board + SQUAREAMOUNT * y + x) != 0){
                int x1 = direction == LEFT? x - 1: x + 1;
                while(1){
                    if(direction == LEFT && x1 < 0 || direction == RIGHT && x1 > SQUAREAMOUNT - 1)
                        break;

                    if(*((int *)board + SQUAREAMOUNT * y + x1) != 0 && *(board + SQUAREAMOUNT * y + x1) == *(board + SQUAREAMOUNT * y + x) && moved_cells[y][x] == 0){
                       *current_score += (*(board + SQUAREAMOUNT * y + x)) * 2;
                       *(board + SQUAREAMOUNT * y + x1) = (*(board + SQUAREAMOUNT * y + x)) * 2;
                       *(board + SQUAREAMOUNT * y + x) = 0;
                       (*occupiedCells)--;
                        moved_cells[y][x1] = 1;
                        break;
                    }


                    if(*(board + SQUAREAMOUNT * y + x1) != 0 && *(board + SQUAREAMOUNT * y + x1)!=*(board + SQUAREAMOUNT * y + x) && moved_cells[y][x] == 0){
                       int value = *(board + 4 * y + x);
                       *(board + SQUAREAMOUNT * y + x) = 0;
                       int Xvalue = direction == LEFT? x1 + 1: x1 - 1;

                       *(board + SQUAREAMOUNT * y + Xvalue) = value;
                       moved_cells[y][Xvalue] = 1;
                       break;
                    }

                    direction == LEFT? --x1: ++x1;
                }
            }

        }

        direction == LEFT? --x: ++x;

     }

     x = direction == LEFT? 1: SQUAREAMOUNT - 2;

     while(1){
        if(direction == LEFT && x > SQUAREAMOUNT - 1 || direction == RIGHT && x < 0)
            break;

        for(int y = 0; y < SQUAREAMOUNT; ++y){
            if(*(board + SQUAREAMOUNT * y + x) != 0){
                int value = *(board + SQUAREAMOUNT * y + x);
                *(board + SQUAREAMOUNT * y + x) = 0;

                int xcor;

                if(direction == LEFT){
                    xcor = 0;
                    //decrease x coordinate to the maximum (move left)
                    while(*(board + SQUAREAMOUNT * y + xcor) != 0 && xcor < SQUAREAMOUNT - 1){
                        xcor++;
                    }
                }

                if(direction == RIGHT){
                    xcor = SQUAREAMOUNT - 1;
                    //increase x coordinate to the maximum (move left)
                        while(*(board + SQUAREAMOUNT * y + xcor) != 0 && xcor > 0){
                            xcor--;
                        }
                    }

                *((int *)board + SQUAREAMOUNT * y + xcor) = value;
            }
        }
        direction== LEFT? ++x: --x;
     }

    if(memcmp(initialBoard, board, SQUAREAMOUNT * SQUAREAMOUNT * sizeof(int))!=0)
        initializeNewValue(board, 1, occupiedCells, NewValueX, NewValueY); // add new value to board
}

void newGame(int *NewValueX, int *NewValueY){
    memset(board, 0, sizeof(board));
    occupiedCells = 0;
    initializeNewValue(&board, 3, &occupiedCells, NewValueX, NewValueY);
    NewValueX = -1;
    NewValueY = -1;
    high_score = current_score > high_score ? current_score : high_score;
    current_score = 0;
}



int main(){
    srand(time(NULL));
    SetConsoleCtrlHandler(ConsoleHandler, TRUE); //if application is closed with "X"

    int InitializeNewValues = 1; // Check if needed to initialize initial values

    //Read from binary file
    FILE* progressFile = fopen(FILENAME, "rb");
    if (progressFile != NULL && fileSize(progressFile) / sizeof(int) == SQUAREAMOUNT * SQUAREAMOUNT + 3) {
        printf("%s\n", FOUNDPROGRESS);
        fread(&board, sizeof(int), SQUAREAMOUNT * SQUAREAMOUNT, progressFile);
        fread(&occupiedCells, sizeof(int), 1, progressFile);
        fread(&current_score, sizeof(int), 1, progressFile);
        fread(&high_score, sizeof(int), 1, progressFile);
        fclose(progressFile);
        InitializeNewValues = 0;
    }else{
        printf("%s\n", NEWGAME);
    }

    int NewValueX, NewValueY; //store cell to highlight
    if(occupiedCells < SQUAREAMOUNT * SQUAREAMOUNT && InitializeNewValues)
        initializeNewValue(&board, 3, &occupiedCells, &NewValueX, &NewValueY);

    NewValueX = -1, NewValueY = -1;
    char move = ' ', clearBuffer = ' ';

    int badInput = 0;


    drawBoard(board, NewValueX, NewValueY, current_score, badInput, high_score);
    while (1) {
        if (occupiedCells == SQUAREAMOUNT * SQUAREAMOUNT && gameEnds(board)) {
            printf("%s", ENDMSG);
            high_score = current_score > high_score ? current_score : high_score;
            newGame(&NewValueX, &NewValueY);
            break;
        }

        if (_kbhit()) {
            move = toupper(getch());
            printf("%c\n", move);
            if (move == UP || move == DOWN || move == RIGHT || move == LEFT || move == 'E' || move == 'N') {
                badInput = 0;

                if (move == UP) { // move up
                    boardMovesVertically(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if (move == DOWN) { // move down
                    boardMovesVertically(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if (move == LEFT) { // move left
                    boardMovesHorizontally(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if (move == RIGHT) { // move right
                    boardMovesHorizontally(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if (move == 'E') { // save and exit
                    saveProgress();
                    break;
                }

                if (move == 'N') { // new game
                    newGame(&NewValueX, &NewValueY);
                }
                system("cls"); // clears console
               drawBoard(board, NewValueX, NewValueY, current_score, badInput, high_score);
            } else {
                system("cls"); // clears console
                badInput = 1; // prints BAD INPUT if TRUE
                drawBoard(board, NewValueX, NewValueY, current_score, badInput, high_score);
            }
        }
    }

    return 0;
}
