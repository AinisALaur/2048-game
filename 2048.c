#include<stdio.h>
#define SQUARESIZE 11

void DrawBoard(int *board[]){
    for(int t=0; t<4; ++t){ //how many total rows
        for(int h=0; h<SQUARESIZE/2; ++h){ //height of 1 row
            for(int r=0; r<4; ++r){ // How many rows
                for(int i=0; i<SQUARESIZE; ++i){ // How many collums
                    if(h==0 || h==SQUARESIZE/2-1 || i==0 || i==SQUARESIZE-1)
                        printf("#");
                    else
                        printf(" ");
                }printf(" ");
            }printf("\n");
        }printf("\n");
    }
}



int main(){

    int board[4][4] = {};
    DrawBoard(board);




    return 0;
}
