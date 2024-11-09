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
                        int sizeOfValue = GetNumSize(*(board+t+4*r));
                        if(sizeOfValue<=3)
                            printf("#%s%5d%3s%s#", COLOR, *(board+t+4*r), " ", COLOR_RESET);
                        else if(sizeOfValue<=5)
                            printf("#%s%6d%2s%s#", COLOR, *(board+t+4*r), " ", COLOR_RESET);
                        else if(sizeOfValue<=7)
                            printf("#%s%7d%1s%s#", COLOR, *(board+t+4*r), " ", COLOR_RESET);
                        else
                            printf("#%s%8d#%s", COLOR, *(board+t+4*r), COLOR_RESET);
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




int main(){
    srand(time(NULL));

    int occupied[4][4] = {};
    int occupiedCells = 0;

    int board[4][4] = {};
    int x, y;

    for(int i=0; i<2; ++i){
        int IsOccupied = 1;
        while(IsOccupied){
            x = rand()%5;
            y = rand()%5;

            if(occupied[x][y] == 0)
                IsOccupied = 0;
        }
        int newCellValue[10] = {4,2,2,2,2,2,2,2,2,2};
        int newIndex = rand()%11;
        occupied[x][y] = -1;
        board[x][y] = newCellValue[newIndex];
        ++occupiedCells;
    }


    DrawBoard(board);



    return 0;
}
