#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SQUARESIZE 10

int GetNumSize(int value){
    int t = 0;

    while(value>0){
        t++;
        value/=10;
    }
    return t;
}


void DrawBoard(int board[4][4]){
    for(int t=0; t<4; ++t){ //how many total rows
        for(int h=0; h<SQUARESIZE/2; ++h){ //height of 1 row
            for(int r=0; r<4; ++r){ // How many rows
                for(int i=0; i<SQUARESIZE; ++i){ // How many collums
                    if (h==SQUARESIZE/4 && i==0){
                        int sizeOfValue = GetNumSize(board[t][r]);
                        if(sizeOfValue<=3)
                            printf("#%5d%3s#", board[t][r], " ");
                        else if(sizeOfValue<=5)
                            printf("#%6d%2s#", board[t][r], " ");
                        else if(sizeOfValue<=7)
                            printf("#%7d%1s#", board[t][r], " ");
                        else
                            printf("#%8d#", board[t][r]);
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
