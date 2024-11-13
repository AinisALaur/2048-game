#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SQUARESIZE 10
#define COLOR  "\x1B[36m"
#define COLOR_RESET  "\x1B[37m"

int GetNumSize(int value){
    int t = 0;
    while(value>0){
        t++;
        value/=10;
    }
    return t;
}

void DrawBoard(int *board){
    for(int t=0; t<4; ++t){ //how many total rows
        for(int h=0; h<SQUARESIZE/2; ++h){ //height of 1 row
            for(int r=0; r<4; ++r){ // How many rows
                for(int i=0; i<SQUARESIZE; ++i){ // How many collums
                    if (h==SQUARESIZE/4 && i==0){ // Write number with correct alignment
                        int sizeOfValue = GetNumSize(*(board+4*t+r));
                        if(sizeOfValue<=3)
                            printf("#%s%5d%3s%s#", COLOR, *(board+4*t+r), " ", COLOR_RESET);
                        else if(sizeOfValue<=5)
                            printf("#%s%6d%2s%s#", COLOR, *(board+4*t+r), " ", COLOR_RESET);
                        else if(sizeOfValue<=7)
                            printf("#%s%7d%1s%s#", COLOR, *(board+4*t+r), " ", COLOR_RESET);
                        else
                            printf("#%s%8d#%s", COLOR, *(board+4*t+r), COLOR_RESET);
                    }
                    if(h>0 && h<SQUARESIZE/2-1 && h!=SQUARESIZE/4 && i==0)
                        printf("#%8s#", " ");
                    if(h==0 || h==SQUARESIZE/2-1)
                        printf("#");
                }printf(" ");
            }printf("\n");
        }printf("\n");
    }
}


void InitializeNewValue(int *board, int NumberOfValues, int *occupiedCells){
    int x, y;
    for(int i=0; i<NumberOfValues; ++i){
        int IsOccupied = 1;
        while(IsOccupied){
            x = rand()%4;
            y = rand()%4;

            if(*(board+4*x+y) == 0)
                IsOccupied = 0;
        }
        int newCellValue[10] = {4,4,4,4,2,2,2,2,2,2};
        int newIndex = rand()%10;
        *(board+4*x+y) = newCellValue[newIndex];
        ++occupiedCells;
    }
}

void BoardMovesVertically(int *board, int *occupiedCells, int start, int end){
    int moved_cells[4][4] = {};

    int i = start;

    while(1){ //starts at bottom row and progresses up

        //check when to stop cycle
        if(start>end && i<=0 || start<end && i>=end){
            break;
        }


        for(int x=0; x<4; ++x){ //goes trough row's elements
            if(*(board+4*i+x)!=0){ // if element is not 0 we look if any non 0 elements exist above/below


                //assign j accordingly
                int  j;
                if(start>end)
                    j = i-1;
                else
                    j = i+1;


                while(1){ // loop that checks all elements above/below

                    if(start>end && j<0 || start<end && j>end){
                        break;
                    }

                    //if finds the same element above it
                    if(*(board+4*j+x)!=0 && *(board+4*j+x)==*(board+4*i+x) && moved_cells[i][x]==0){
                       *(board+4*j+x) = (*(board+4*j+x)) * (*(board+4*i+x));
                       *(board+4*i+x) = 0;
                       *occupiedCells--;
                        moved_cells[j][x] = 1; // make sure to not move already moved values
                        break;
                    }

                    //if finds a non 0 element that is not equal to original
                    else if(*(board+4*j+x)!=0 && *(board+4*j+x)!=*(board+4*i+x) && moved_cells[i][x]==0){
                       int new_value = *(board+4*i+x);
                       *(board+4*i+x) = 0;
                       *(board+4*(j+1)+x) = new_value;
                       moved_cells[j+1][x] = 1; // make sure to not move already moved values
                       break;
                    }


                    //move to upper or lower row
                    if(start>end)
                        --j;
                    else
                        ++j;

                }
            }
        }


        //move up or down
        if(start>end)
            --i;
        else
            ++i;
    }


    //assign appropriate i value for upcoming loop
    if(start>end)
        i = 1;
    else
        i = 2;

    //Move all elements up/down
    while(1){ // start at second row from top and progress down or second row from bottom

        //break function when needed
        if(start>end && i>3 || start<end && i<0){
            break;
        }


        for(int x=0; x<4; ++x){ // go through all row's elements
            int cellValue = *(board+4*i+x);
            if(cellValue !=0){ // if element is not zero move it as high/low up as possible
                *(board+4*i+x) = 0;


                int ValuesYcord;

                if(start>end){
                    ValuesYcord = 0;
                    while(*(board+4*ValuesYcord+x)!=0 && ValuesYcord<=3) // increasing it's y coordinate to the maximum
                        ValuesYcord++;
                }
                else{
                    ValuesYcord = 3;
                    while(*(board+4*ValuesYcord+x)!=0 && ValuesYcord>=0) // decreasing it's y coordinate to the maximum
                        ValuesYcord--;
                }

                *(board+4*ValuesYcord+x) = cellValue;
            }
        }


       // move up a row or down
       if(start>end)
          ++i;
       else
          --i;

    }

    InitializeNewValue(board, 1, occupiedCells); // add new value to board

}




int main(){
    srand(time(NULL));
    int occupiedCells = 0;
    int board[4][4] = {};

    InitializeNewValue(&board, 3, &occupiedCells);


    int GameContinues = 1;
    char move, clearBuffer;
    while(GameContinues){
        DrawBoard(&board);

        printf("u - up\n");
        printf("d - down\n");
        printf("l - left\n");
        printf("r - right\n");
        printf("Enter your move: ");

        if(scanf("%c", &move)==1 && getchar()=='\n'){
            move = toupper(move);
            if(move == 'U' || move == 'D' || move == 'R' || move == 'L'){
                printf("Move registered!\n");


                if(move == 'U'){
                    BoardMovesVertically(&board, &occupiedCells, 3, 0);
                }

                if(move == 'D'){
                    BoardMovesVertically(&board, &occupiedCells, 0, 3);
                }


            }
            else{
                printf("Bad input!\n");
                while(clearBuffer=getchar()!='\n' && clearBuffer!=EOF);
            }
        }

        else{
            printf("Bad input!\n");
            while(clearBuffer=getchar()!='\n' && clearBuffer!=EOF);
        }

        printf("\n");
   }

    return 0;
}
