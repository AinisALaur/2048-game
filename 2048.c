#include<stdio.h>
#define SQUARESIZE 11

void DrawBoard(int board[4][4]){
    for(int t=0; t<4; ++t){ //how many total rows
        for(int h=0; h<SQUARESIZE/2; ++h){ //height of 1 row
            for(int r=0; r<4; ++r){ // How many rows
                for(int i=0; i<SQUARESIZE; ++i){ // How many collums
                    //printf("%d", i);
                    if (h==SQUARESIZE/4 && i==0)
                        printf("#%9d#", 1, " ");

                    if(h>0 && h<SQUARESIZE/2-1 && h!=SQUARESIZE/4 && i==0)
                        printf("#%9s#", " ");


                    if(h==0 || h==SQUARESIZE/2-1)
                        printf("#");

                }printf(" ");
            }printf("\n");
        }printf("\n");
    }
}



int main(){

    int board[4][4] = {};
    for(int i=0; i<4; ++i){
        for(int x=0; x<4; ++x){
            board[i][x] = 1;
        }

    }

    DrawBoard(board);




    return 0;
}
