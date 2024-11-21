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

//TODO clean up code, comment code, if square mount doesn't match to squaremount new game, save high-score


//make global variables so when program shuts down unexpectedly - saves progress correctly
int board[SQUAREAMOUNT][SQUAREAMOUNT] = {};
int occupiedCells = 0;
int current_score = 0;
int high_score = 0;


//handle when program shuts down unexpectedly
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT) {
        SaveProgress();
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
int GetNumSize(int value){
    int t = 0;
    while(value>0){
        t++;
        value/=10;
    }
    return t;
}

//save progress to file
void SaveProgress(){
    FILE* progressFile = fopen("Progress.dat", "wb");
    if (progressFile!=NULL) {
        fwrite(&board, sizeof(int), SQUAREAMOUNT*SQUAREAMOUNT, progressFile);
        fwrite(&occupiedCells, sizeof(int), 1, progressFile);
        fwrite(&current_score, sizeof(int), 1, progressFile);
        fwrite(&high_score, sizeof(int), 1, progressFile);
        fclose(progressFile);
    }
}


//Check if no more available moves
int GameEnds(void *board) {
    for (int i = 0; i < SQUAREAMOUNT; ++i) {
        for (int x = 0; x < SQUAREAMOUNT; ++x) {
            int current = *((int *)board + SQUAREAMOUNT * i + x);

            // Check left neighbor
            if (x > 0 && current == *((int *)board + SQUAREAMOUNT * i + (x - 1))) {
                return 0;
            }

            // Check right neighbor
            if (x < SQUAREAMOUNT-1 && current == *((int *)board + SQUAREAMOUNT * i + (x + 1))) {
                return 0;
            }

            // Check top neighbor
            if (i > 0 && current == *((int *)board + SQUAREAMOUNT * (i - 1) + x)) {
                return 0;
            }

            // Check bottom neighbor
            if (i < SQUAREAMOUNT-1 && current == *((int *)board + SQUAREAMOUNT * (i + 1) + x)) {
                return 0;
            }
        }
    }

    return 1;
}

// print a straight horizontal line for board visualization
void straightLine(int length){
    for(int i=0; i<SQUAREAMOUNT; ++i){
        for(int x=0; x<length; ++x){
            printf("-", x);
        }
    }printf("-\n");
}


//draw game board
void drawBoard(void *board, int newValueX, int newValueY, int current_score, int badInput, int high_score){
    for(int j=0; j<SQUAREAMOUNT; ++j){
        straightLine(SQUARESIZE);
        for(int i=0; i<SQUARESIZE/2; ++i){
            for(int x=0; x<SQUAREAMOUNT; ++x){
                if(i==SQUARESIZE/4 && *((int *)board+SQUAREAMOUNT*j+x)!=0){

                    //calculate padding to align number to center
                    int cellWidth = SQUARESIZE-1;
                    int sizeOfNum = GetNumSize(*((int *)board+SQUAREAMOUNT*j+x));
                    int leftPadding = (cellWidth-sizeOfNum)/2;
                    int rightPadding = cellWidth-leftPadding-sizeOfNum;



                    if(cellWidth<sizeOfNum){
                        printf("THE VALUES DO NOT FIT IN THE CELLS, PLEASE SELECT A BIGGER CELL SIZE!!");
                        exit(0);
                    }


                    char color[] = COLOR;
                    if(j==newValueY && x==newValueX)
                        strcpy(color,HIGHLIGHTCOLOR);

                    printf("|%s%*s%d%s%*s", color,leftPadding,"",*((int *)board+SQUAREAMOUNT*j+x),COLOR_RESET,rightPadding,"");
                }
                else{
                    printf("|%*s",SQUARESIZE-1,"");
                }
            }

            if(j==0 && i==0)
                printf("|  2048 GAME\n");
            else if(j==0 && i==1)
                printf("|  MADE AND DESIGNED BY AINIS AUGUSTAS LAURINAVICIUS\n");
            else if(j==1 && i==0)
                printf("|  %sSCORE: %d%s\n", SCORECOLOR,current_score,COLOR_RESET);
            else if(j==1 && i==1)
                printf("|  %sHIGH SCORE: %d%s\n", SCORECOLOR,high_score>current_score?high_score:current_score,COLOR_RESET);
            else if(j==1 && i==1 && badInput)
                printf("|  %sBAD INPUT%s\n", BADINPUTCOLOR,COLOR_RESET);
            else
                printf("|\n");
        }
    }
    straightLine(SQUARESIZE);
}


int InitializeNewValue(void *board, int NumberOfValues, int *occupiedCells, int *NewValueX, int *NewValueY){
    int x, y;
    for(int i=0; i<NumberOfValues; ++i){
        int IsOccupied = 1;
        while(IsOccupied){
            x = rand()%SQUAREAMOUNT;
            y = rand()%SQUAREAMOUNT;

            if(*((int *)board+SQUAREAMOUNT*y+x) == 0)
                IsOccupied = 0;
        }
        int newCellValue[10] = {4,2,2,2,2,2,2,2,2,2}; // 4 has a 10% to appear
        int newIndex = rand()%10;
        *((int *)board+SQUAREAMOUNT*y+x) = newCellValue[newIndex];
        (*occupiedCells) +=1;
        *NewValueX = x;
        *NewValueY = y;
    }
}

void BoardMovesVertically(void *board, int *occupiedCells, char direction, int *NewValueX, int *NewValueY, int *current_score){
    int moved_cells[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    int y = direction=='U'?SQUAREAMOUNT-1:0;

    int initialBoard[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    memcpy(initialBoard, board, SQUAREAMOUNT * SQUAREAMOUNT *sizeof(int));

    while(1){ //starts at bottom row and progresses up

        //check when to stop cycle
        if(direction=='U' && y<=0 || direction=='D' && y>=SQUAREAMOUNT-1){
            break;
        }

        for(int x=0; x<SQUAREAMOUNT; ++x){ //goes trough row's elements
            if(*((int *)board+SQUAREAMOUNT*y+x)!=0){ // if element is not 0 we look if any non 0 elements exist above/below

                //assign j accordingly
                int  y1 = direction=='U'? y-1: y+1;

                while(1){ // loop that checks all elements above/below
                    if(direction=='U' && y1<0 || direction=='D' && y1>SQUAREAMOUNT-1){
                        break;
                    }

                    //if finds the same element above it
                    if(*((int *)board+SQUAREAMOUNT*y1+x)!=0 && *((int *)board+SQUAREAMOUNT*y1+x)==*((int *)board+SQUAREAMOUNT*y+x) && moved_cells[y][x]==0){
                       *current_score += (*((int *)board+SQUAREAMOUNT*y1+x)) * 2;
                       *((int *)board+SQUAREAMOUNT*y1+x) = (*((int *)board+SQUAREAMOUNT*y1+x)) * 2;
                       *((int *)board+SQUAREAMOUNT*y+x) = 0;
                       (*occupiedCells)--;
                        moved_cells[y1][x] = 1; // make sure to not move already moved values
                        break;
                    }

                    //if finds a non 0 element that is not equal to original
                    else if(*((int *)board+SQUAREAMOUNT*y1+x)!=0 && *((int *)board+SQUAREAMOUNT*y1+x)!=*((int *)board+SQUAREAMOUNT*y+x) && moved_cells[y][x]==0){
                       int new_value = *((int *)board+SQUAREAMOUNT*y+x);
                       *((int *)board+SQUAREAMOUNT*y+x) = 0;
                       int Yvalue = direction=='U'? y1+1: y1-1;

                       *((int *)board+SQUAREAMOUNT*Yvalue+x) = new_value;
                       moved_cells[Yvalue][x] = 1; // make sure to not move already moved values
                       break;
                    }

                    //move to upper or lower row
                    direction=='U'? --y1:++y1;
                }
            }
        }

        //move up or down
        direction=='U'? --y: ++y;
    }

    //assign appropriate i value for upcoming loop
    y=direction=='U'?1:SQUAREAMOUNT-2;

    //Move all elements up/down
    while(1){ // start at second row from top and progress down or second row from bottom

        //break function when needed
        if(direction=='U' && y>SQUAREAMOUNT-1 || direction=='D' && y<0){
            break;
        }

        for(int x=0; x<SQUAREAMOUNT; ++x){ // go through all row's elements
            int cellValue = *((int *)board+SQUAREAMOUNT*y+x);
            if(cellValue !=0){ // if element is not zero move it as high/low up as possible
                *((int *)board+SQUAREAMOUNT*y+x) = 0;
                int ValuesYcord;
                if(direction=='U'){
                    ValuesYcord = 0;
                    while(*((int *)board+SQUAREAMOUNT*ValuesYcord+x)!=0) // increasing it's y coordinate to the maximum
                        ValuesYcord++;
                }
                else{
                    ValuesYcord = SQUAREAMOUNT-1;
                    while(*((int *)board+SQUAREAMOUNT*ValuesYcord+x)!=0) // decreasing it's y coordinate to the maximum
                        ValuesYcord--;
                }

                *((int *)board+SQUAREAMOUNT*ValuesYcord+x) = cellValue;
            }
        }
       // move up a row or down
       direction=='U'? ++y:--y;
    }

    if(memcmp(initialBoard, board, SQUAREAMOUNT*SQUAREAMOUNT*sizeof(int))!=0)
        InitializeNewValue(board, 1, occupiedCells, NewValueX, NewValueY); // add new value to board
}

void boardMovesHorizontally(void *board, int *occupiedCells, char direction, int *NewValueX, int *NewValueY, int *current_score){
    int moved_cells[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    int x = direction == 'L'? SQUAREAMOUNT-1: 0;

    int initialBoard[SQUAREAMOUNT][SQUAREAMOUNT] = {};
    memcpy(initialBoard, board, SQUAREAMOUNT * SQUAREAMOUNT *sizeof(int));


    //move left or right
    while(1){

        if(direction == 'L' && x<1 || direction=='R' && x>SQUAREAMOUNT-1)
            break;

        for(int y=0; y<SQUAREAMOUNT; ++y){
            if(*((int *)board+SQUAREAMOUNT*y+x) != 0){
                int x1 = direction=='L'? x-1: x+1;
                while(1){
                    if(direction=='L' && x1<0 || direction=='R' && x1>SQUAREAMOUNT-1)
                        break;

                    if(*((int *)board+SQUAREAMOUNT*y+x1)!=0 && *((int *)board+SQUAREAMOUNT*y+x1)==*((int *)board+SQUAREAMOUNT*y+x) && moved_cells[y][x]==0){
                       *current_score += (*((int *)board+SQUAREAMOUNT*y+x)) * 2;
                       *((int *)board+SQUAREAMOUNT*y+x1) = (*((int *)board+SQUAREAMOUNT*y+x)) * 2;
                       *((int *)board+SQUAREAMOUNT*y+x) = 0;
                       (*occupiedCells)--;
                        moved_cells[y][x1] = 1;
                        break;
                    }


                    if(*((int *)board+SQUAREAMOUNT*y+x1)!=0 && *((int *)board+SQUAREAMOUNT*y+x1)!=*((int *)board+SQUAREAMOUNT*y+x) && moved_cells[y][x]==0){
                       int value = *((int *)board+4*y+x);
                       *((int *)board+SQUAREAMOUNT*y+x) = 0;
                       int Xvalue = direction=='L'?x1+1:x1-1;

                       *((int *)board+SQUAREAMOUNT*y+Xvalue) = value;
                       moved_cells[y][Xvalue] = 1;
                       break;
                    }

                    direction=='L'?--x1: ++x1;
                }
            }

        }

        direction=='L'? --x: ++x;

     }

     x = direction=='L'? 1: SQUAREAMOUNT-2;

     while(1){
        if(direction=='L' && x>SQUAREAMOUNT-1 || direction=='R' && x<0)
            break;

        for(int y=0; y<SQUAREAMOUNT; ++y){
            if(*((int *)board+SQUAREAMOUNT*y+x)!=0){
                int value = *((int *)board+SQUAREAMOUNT*y+x);
                *((int *)board+SQUAREAMOUNT*y+x) = 0;

                int xcor;

                if(direction=='L'){
                    xcor = 0;
                    //decrease x coordinate to the maximum (move left)
                    while(*((int *)board+SQUAREAMOUNT*y+xcor)!=0 && xcor<SQUAREAMOUNT-1){
                        xcor++;
                    }
                }

                if(direction=='R'){
                    xcor = SQUAREAMOUNT-1;
                    //increase x coordinate to the maximum (move left)
                        while(*((int *)board+SQUAREAMOUNT*y+xcor)!=0 && xcor>0){
                            xcor--;
                        }
                    }

                *((int *)board+SQUAREAMOUNT*y+xcor) = value;
            }
        }
        direction=='L'?++x: --x;
     }

    if(memcmp(initialBoard, board, SQUAREAMOUNT*SQUAREAMOUNT*sizeof(int))!=0)
        InitializeNewValue(board, 1, occupiedCells, NewValueX, NewValueY); // add new value to board
}



int main(){
    srand(time(NULL));
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    int InitializeNewValues = 1;

    //Read from binary file
    FILE* progressFile = fopen("Progress.dat", "rb");
    if (progressFile != NULL && fileSize(progressFile)/sizeof(int)==SQUAREAMOUNT*SQUAREAMOUNT+3) {
        printf("Game progress found!\n");
        fread(&board, sizeof(int), SQUAREAMOUNT * SQUAREAMOUNT, progressFile);
        fread(&occupiedCells, sizeof(int), 1, progressFile);
        fread(&current_score, sizeof(int), 1, progressFile);
        fread(&high_score, sizeof(int), 1, progressFile);
        fclose(progressFile);
        InitializeNewValues = 0;
    }else{
        printf("Starting a new game!\n");
    }



    int NewValueX, NewValueY; //store cell to highlight
    if(occupiedCells<SQUAREAMOUNT*SQUAREAMOUNT && InitializeNewValues)
        InitializeNewValue(&board, 3, &occupiedCells, &NewValueX, &NewValueY);


    NewValueX=-1, NewValueY=-1;
    char move, clearBuffer;

    int badInput = 0;

    while(1){
        drawBoard(board,NewValueX,NewValueY,current_score, badInput, high_score);

        if(occupiedCells==SQUAREAMOUNT*SQUAREAMOUNT && GameEnds(board)){
            printf("Game has ended");
            high_score = current_score>high_score?current_score:high_score;
            SaveProgress();
            break;
        }

        //printf("Current score: %d\n", current_score);
        printf("u - up     e - save progress and exit\n");
        printf("d - down   n - new game\n");
        printf("l - left\n");
        printf("r - right\n");
        printf("Enter your move: ");

        if(scanf("%c", &move)==1 && getchar()=='\n'){
            move = toupper(move);
            if(move == 'U' || move == 'D' || move == 'R' || move == 'L' || move == 'E' || move == 'N'){
                badInput = 0;
                printf("Move registered!\n");

                if(move == 'U'){
                    BoardMovesVertically(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if(move == 'D'){
                    BoardMovesVertically(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if(move == 'L'){
                    boardMovesHorizontally(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if(move == 'R'){
                    boardMovesHorizontally(&board, &occupiedCells, move, &NewValueX, &NewValueY, &current_score);
                }

                if(move == 'E'){
                    SaveProgress();
                    break;
                }

                if(move == 'N'){
                    memset(board, 0, sizeof(board));
                    occupiedCells = 0;
                    InitializeNewValue(&board, 3, &occupiedCells, &NewValueX, &NewValueY);
                    NewValueX = -1; NewValueY = -1;
                    high_score = current_score>high_score?current_score:high_score;
                    current_score = 0;
                }

            }
            else{
                badInput = 1;
            }
        }

        else{
            badInput = 1;
            while(clearBuffer=getchar()!='\n' && clearBuffer!=EOF);
        }
        system("cls"); //clears console
   }

    return 0;
}
