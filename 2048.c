#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SQUARESIZE 10
#define COLOR  "\x1B[31m"
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


void InitializeNewValue(int *board, int NumberOfValues, int *occupied, int *occupiedCells){
    int x, y;
    for(int i=0; i<NumberOfValues; ++i){
        int IsOccupied = 1;
        while(IsOccupied){
            x = rand()%5;
            y = rand()%5;

            if(*(occupied+4*y+x) == 0)
                IsOccupied = 0;
        }
        int newCellValue[10] = {4,2,2,2,2,2,2,2,2,2};
        int newIndex = rand()%10;
        *(occupied+4*x+y) = 1;
        *(board+4*x+y) = newCellValue[newIndex];
        ++occupiedCells;
    }
}

void BoardMovesUp(int *board, int *occupied, int *occupiedCells){
    for(int i=3; i>0; --i){
        for(int x=0; x<4; ++x){
            if(*(board+4*i+x)!=0){
                    printf("found %d at coordinates %d %d\n", *(board+4*i+x), i, x);


//                for(int j=i; j>0; --j){
//
//                    if(*(board+4*j+x)!=0 && *(board+4*j+x)==*(board+4*i+x)){
//
//                       *(board+4*j+x) = (*(board+4*j+x)) * (*(board+4*i+x));
//                       *(board+4*i+x) = 0;
//                       *(occupied+4*i+x) = 0;
//                       *(occupied+4*j+x) = 1;
//                       *occupiedCells--;
//                        break;
//
//                    }


//                    else if(*(board+4*j+x)!=0 && *(board+4*j+x)!=*(board+4*i+x)){
//                       *(board+4*(j-1)+x) = *(board+4*i+x);
//                       *(board+4*i+x) = 0;
//                       *(occupied+4*i+x) = 0;
//                       *(occupied+4*(j-1)+x) = 1;
//                       break;
//                    }
//
//                    else{
//                       *(board+x) = *(board+4*i+x);
//                       *(board+4*i+x) = 0;
//                       *(occupied+4*i+x) = 0;
//                       *(occupied+x) = 1;
//                       break;
//                    }
                }
//            }
        }
    }
    //InitializeNewValue(*board, 1, *occupied, *occupiedCells);
}




int main(){
    srand(time(NULL));

    int occupied[4][4] = {};
    int occupiedCells = 0;
    int board[4][4] = {};

    InitializeNewValue(&board, 2, &occupied, &occupiedCells);
    //DrawBoard(&board);

    printf("u - up\n");
    printf("d - down\n");
    printf("l - left\n");
    printf("r - right\n");

    int GameContinues = 1;
    char move, clearBuffer;
    while(GameContinues){
        for(int i=0; i<4; ++i){
            for(int x=0; x<4; ++x){
                printf("%d ", board[i][x]);

            }printf("\n");
        }

        printf("Enter your move: ");

        if(scanf("%c", &move)==1 && getchar()=='\n'){
            move = toupper(move);
            if(move == 'U' || move == 'D' || move == 'R' || move == 'L'){
                printf("Move registered!\n");
                if(move == 'U')
                    BoardMovesUp(&board, &occupied, &occupiedCells);


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
        //DrawBoard(&board);

   }




    return 0;
}
