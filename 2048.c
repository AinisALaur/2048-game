#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SQUARESIZE 10 //tested with 11
#define COLOR  "\x1B[36m"
#define SCORECOLOR  "\x1B[33m"
#define HIGHLIGHTCOLOR  "\x1B[31m"
#define BADINPUTCOLOR  "\x1B[31m"
#define COLOR_RESET  "\x1B[37m"

//TODO clean up code, comment code

int GetNumSize(int value){
    int t = 0;
    while(value>0){
        t++;
        value/=10;
    }
    return t;
}

int boardsAreEqual(int *board1, int *board2){
    for(int i=0; i<4; ++i){
        for(int x=0; x<4; ++x){
            if(*(board1+4*i+x) != *(board2+4*i+x))
                return 0;
        }
    }
    return 1;
}


void copyBoards(int *board1, int *board2){
    for(int i=0; i<4; ++i){
        for(int x=0; x<4; ++x){
            *(board1+4*i+x) = *(board2+4*i+x);
        }
    }
}

int GameEnds(int *board){
    int noMoreMoves = 0;

    for(int i=0; i<4; ++i){
        for(int x=0; x<4; ++x){
            if(i==0){
                if(*(board+4*i+x) == *(board+4*(i+1)+x))
                    return 0;
            }

            if(x==0){
                if(*(board+4*i+x) == *(board+4*(i-1)+(x+1)))
                    return 0;
            }

            if(i>0 && i<3){
                if(*(board+4*i+x) == *(board+4*(i-1)+x))
                    return 0;

                if(*(board+4*i+x) == *(board+4*(i+1)+x))
                    return 0;
            }

            if(x>0 && x<3){
                if(*(board+4*i+x) == *(board+4*(i-1)+(x-1)))
                    return 0;

                if(*(board+4*i+x) == *(board+4*(i-1)+(x+1)))
                    return 0;
            }

            if(x==3){
                if(*(board+4*i+x) == *(board+4*(i-1)+(x-1)))
                    return 0;
            }


            if(i==3){
                if(*(board+4*i+x) == *(board+4*(i-1)+x))
                    return 0;
            }
        }
    }

    return 1;
}


void straightLine(int length){
    for(int i=0; i<4; ++i){
        for(int x=0; x<length; ++x){
            printf("-", x);
        }
    }printf("-\n");
}

void drawBoard(int *board, int newValueX, int newValueY, int high_score, int badInput){

    for(int j=0; j<4; ++j){
        straightLine(SQUARESIZE);
        for(int i=0; i<SQUARESIZE/2; ++i){
            for(int x=0; x<4; ++x){
                if(i==SQUARESIZE/4 && *(board+4*j+x)!=0){
                    int cellWidth = SQUARESIZE-1;
                    int sizeOfNum = GetNumSize(*(board+4*j+x));
                    int leftPadding = (cellWidth-sizeOfNum)/2;
                    int rightPadding = cellWidth-leftPadding-sizeOfNum;

                    if(cellWidth<sizeOfNum){
                        printf("THE VALUES DO NOT FIT IN THE CELLS, PLEASE SELECT A BIGGER SIZE!!");
                        exit(0);
                    }


                    char color[] = COLOR;
                    if(j==newValueY && x==newValueX)
                        strcpy(color,HIGHLIGHTCOLOR);

                    printf("|%s%*s%d%s%*s", color,leftPadding,"",*(board+4*j+x),COLOR_RESET,rightPadding,"");
                }
                else{
                    printf("|%*s",SQUARESIZE-1,"");
                }
            }

            if(j==0 && i==0)
                printf("|  2048 GAME\n");
            else if(j==0 && i==1)
                printf("|  MADE AND DESIGNED BY AINIS AUGUSTAS LAURINAVICIUS\n");
            else if(j==0 && i==3)
                printf("|  %sCURRENT SCORE: %d%s\n", SCORECOLOR,high_score,COLOR_RESET);
            else if(j==0 && i==4 && badInput)
                printf("|  %sBAD INPUT%s\n", BADINPUTCOLOR,COLOR_RESET);
            else
                printf("|\n");
        }
    }
    straightLine(SQUARESIZE);
}








int InitializeNewValue(int *board, int NumberOfValues, int *occupiedCells, int *NewValueX, int *NewValueY){
    int x, y;
    for(int i=0; i<NumberOfValues; ++i){
        int IsOccupied = 1;
        while(IsOccupied){
            x = rand()%4;
            y = rand()%4;

            if(*(board+4*y+x) == 0)
                IsOccupied = 0;
        }
        int newCellValue[10] = {4,2,2,2,2,2,2,2,2,2}; // 4 has a 10% to appear
        int newIndex = rand()%10;
        *(board+4*y+x) = newCellValue[newIndex];
        (*occupiedCells) +=1;
        *NewValueX = x;
        *NewValueY = y;
    }
}

void BoardMovesVertically(int *board, int *occupiedCells, char direction, int *NewValueX, int *NewValueY, int *high_score){
    int moved_cells[4][4] = {};
    int y = direction=='U'?3:0;

    int initialBoard[4][4];
    copyBoards(initialBoard, board);

    while(1){ //starts at bottom row and progresses up

        //check when to stop cycle
        if(direction=='U' && y<=0 || direction=='D' && y>=3){
            break;
        }

        for(int x=0; x<4; ++x){ //goes trough row's elements
            if(*(board+4*y+x)!=0){ // if element is not 0 we look if any non 0 elements exist above/below

                //assign j accordingly
                int  y1 = direction=='U'? y-1: y+1;

                while(1){ // loop that checks all elements above/below
                    if(direction=='U' && y1<0 || direction=='D' && y1>3){
                        break;
                    }

                    //if finds the same element above it
                    if(*(board+4*y1+x)!=0 && *(board+4*y1+x)==*(board+4*y+x) && moved_cells[y][x]==0){
                       *high_score += (*(board+4*y1+x)) * 2;
                       *(board+4*y1+x) = (*(board+4*y1+x)) * 2;
                       *(board+4*y+x) = 0;
                       (*occupiedCells)--;
                        moved_cells[y1][x] = 1; // make sure to not move already moved values
                        break;
                    }

                    //if finds a non 0 element that is not equal to original
                    else if(*(board+4*y1+x)!=0 && *(board+4*y1+x)!=*(board+4*y+x) && moved_cells[y][x]==0){
                       int new_value = *(board+4*y+x);
                       *(board+4*y+x) = 0;
                       int Yvalue = direction=='U'? y1+1: y1-1;

                       *(board+4*Yvalue+x) = new_value;
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
    y=direction=='U'?1:2;

    //Move all elements up/down
    while(1){ // start at second row from top and progress down or second row from bottom

        //break function when needed
        if(direction=='U' && y>3 || direction=='D' && y<0){
            break;
        }

        for(int x=0; x<4; ++x){ // go through all row's elements
            int cellValue = *(board+4*y+x);
            if(cellValue !=0){ // if element is not zero move it as high/low up as possible
                *(board+4*y+x) = 0;
                int ValuesYcord;
                if(direction=='U'){
                    ValuesYcord = 0;
                    while(*(board+4*ValuesYcord+x)!=0) // increasing it's y coordinate to the maximum
                        ValuesYcord++;
                }
                else{
                    ValuesYcord = 3; //delete
                    while(*(board+4*ValuesYcord+x)!=0) // decreasing it's y coordinate to the maximum
                        ValuesYcord--;
                }

                *(board+4*ValuesYcord+x) = cellValue;
            }
        }
       // move up a row or down
       direction=='U'? ++y:--y;
    }

    if(boardsAreEqual(initialBoard, board)==0)
        InitializeNewValue(board, 1, occupiedCells, NewValueX, NewValueY); // add new value to board
}

void boardMovesHorizontally(int *board, int *occupiedCells, char direction, int *NewValueX, int *NewValueY, int *high_score){
    int moved_cells[4][4] = {};
    int x = direction == 'L'? 3: 0;

    int initialBoard[4][4];
    copyBoards(initialBoard, board);


    //move left or right
    while(1){

        if(direction == 'L' && x<1 || direction=='R' && x>3)
            break;

        for(int y=0; y<4; ++y){
            if(*(board+4*y+x) != 0){
                int x1 = direction=='L'? x-1: x+1;
                while(1){
                    if(direction=='L' && x1<0 || direction=='R' && x1>3)
                        break;

                    if(*(board+4*y+x1)!=0 && *(board+4*y+x1)==*(board+4*y+x) && moved_cells[y][x]==0){
                       *high_score += (*(board+4*y+x)) * 2;
                       *(board+4*y+x1) = (*(board+4*y+x)) * 2;
                       *(board+4*y+x) = 0;
                       (*occupiedCells)--;
                        moved_cells[y][x1] = 1;
                        break;
                    }


                    if(*(board+4*y+x1)!=0 && *(board+4*y+x1)!=*(board+4*y+x) && moved_cells[y][x]==0){
                       int value = *(board+4*y+x);
                       *(board+4*y+x) = 0;
                       int Xvalue = direction=='L'?x1+1:x1-1;

                       *(board+4*y+Xvalue) = value;
                       moved_cells[y][Xvalue] = 1;
                       break;
                    }

                    direction=='L'?--x1: ++x1;
                }
            }

        }

        direction=='L'? --x: ++x;

     }

     x = direction=='L'? 1: 2;

     while(1){
        if(direction=='L' && x>3 || direction=='R' && x<0)
            break;

        for(int y=0; y<4; ++y){
            if(*(board+4*y+x)!=0){
                int value = *(board+4*y+x);
                *(board+4*y+x) = 0;

                int xcor;

                if(direction=='L'){
                    xcor = 0;
                    //decrease x coordinate to the maximum (move left)
                    while(*(board+4*y+xcor)!=0 && xcor<3){
                        xcor++;
                    }
                }

                if(direction=='R'){
                    xcor = 3;
                    //increase x coordinate to the maximum (move left)
                        while(*(board+4*y+xcor)!=0 && xcor>0){
                            xcor--;
                        }
                    }

                *(board+4*y+xcor) = value;
            }
        }
        direction=='L'?++x: --x;
     }

     if(boardsAreEqual(initialBoard, board)==0)
        InitializeNewValue(board, 1, occupiedCells, NewValueX, NewValueY); // add new value to board
}



int main(){
    srand(time(NULL));
    int occupiedCells = 0;
    int board[4][4] = {};
    int NewValueX, NewValueY; //store cell to highlight

    int high_score = 0;

    InitializeNewValue(&board, 3, &occupiedCells, &NewValueX, &NewValueY);
    NewValueX=-1, NewValueY=-1;
    char move, clearBuffer;

    int badInput = 0;

    while(1){
        drawBoard(board,NewValueX,NewValueY,high_score, badInput);

        if(occupiedCells==16 && GameEnds(board)){
            printf("Game has ended");
            break;
        }


        //printf("Current score: %d\n", high_score);
        printf("u - up\n");
        printf("d - down\n");
        printf("l - left\n");
        printf("r - right\n");
        printf("Enter your move: ");

        if(scanf("%c", &move)==1 && getchar()=='\n'){
            move = toupper(move);
            if(move == 'U' || move == 'D' || move == 'R' || move == 'L'){
                badInput = 0;
                printf("Move registered!\n");

                if(move == 'U'){
                    BoardMovesVertically(&board, &occupiedCells, move, &NewValueX, &NewValueY, &high_score);
                }

                if(move == 'D'){
                    BoardMovesVertically(&board, &occupiedCells, move, &NewValueX, &NewValueY, &high_score);
                }

                if(move == 'L'){
                    boardMovesHorizontally(&board, &occupiedCells, move, &NewValueX, &NewValueY, &high_score);
                }

                if(move == 'R'){
                    boardMovesHorizontally(&board, &occupiedCells, move, &NewValueX, &NewValueY, &high_score);
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
        system("cls");
   }

    return 0;
}
