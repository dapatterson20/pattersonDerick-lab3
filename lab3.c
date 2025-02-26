#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    int** board = NULL;

    sudoku_board=(int**)malloc(sizeof(int*)*ROW_SIZE);
    for (int row=0;row<ROW_SIZE;row++) {
        sudoku_board[row]=(int*)malloc(sizeof(int)*COL_SIZE);
    }

    for (int i=0; i<ROW_SIZE*COL_SIZE; i++) {
        fscanf(fp, "%d,", &sudoku_board[i/9][i%9]);
    }

    // replace this comment with your code

    return sudoku_board;
}

void* checkRow(void* param) {
    param_struct* arg=(param_struct*) param;
    int row=arg->starting_row;
    int inArr[9]={0,0,0,0,0,0,0};
    for (int x=0; x<9; x++) {
        if (sudoku_board[row][x]<10 && sudoku_board[row][x]>0) {
            inArr[sudoku_board[row][x]-1]+=1;
            if (inArr[sudoku_board[row][x]-1]>1) {
                worker_validation=(int*)0;
                return NULL;
            }
        }
        else {
            worker_validation=(int*)0;
            return NULL;
        }
    }
    for (int z=0; z<9; z++) {
        if (inArr[z]!=1) {
            worker_validation=(int*)0;
            return NULL;
        }
    }
    worker_validation=(int*)1;
    return NULL;
}

void* checkCol(void* param) {
    param_struct* arg=(param_struct*) param;
    int index=arg->starting_col;
    int inArr[9]={0,0,0,0,0,0,0};
    for (int x=0; x<9; x++) {
        if (sudoku_board[x][index]<10 && sudoku_board[x][index]>0) {
            inArr[sudoku_board[x][index]-1]++;
            if (inArr[sudoku_board[x][index]-1]>1) {
                worker_validation=(int*)0;
                return NULL;
            }
        }
        else {
            worker_validation=(int*)0;
            return NULL;
        }
    }

    for (int z=0; z<9; z++) {
        if (inArr[z]!=1) {
            worker_validation=(int*)0;
            return NULL;
        }
    }
    worker_validation=(int*)1;
    return NULL;
}

void* checkSquare(void* param) {
    param_struct* arg=(param_struct*) param;
    int maxRow=arg->ending_row;
    int maxCol=arg->ending_col;
    int inArr[9]={0,0,0,0,0,0,0};
    for (int x=maxCol-3; x<maxCol && x>=maxCol-3; x++) {
        for (int y=maxRow-3; y<maxRow && y>=maxRow-3; y++) {
            if (sudoku_board[x][y]<10 && sudoku_board[x][y]>0) {
                inArr[sudoku_board[x][y]-1]++;
                if (inArr[sudoku_board[x][y]-1]>1) {
                    worker_validation=(int*)0;
                    return NULL;
                }
            }
            else {
                worker_validation=(int*)0;
                return NULL;
            }
        }
    }
    
    for (int z=0; z<9;z++) {
        if (inArr[z]!=1) {
            worker_validation=(int*)0;
            return NULL;
        }
    }
    worker_validation=(int*)1;
    return NULL;
}


int is_board_valid() {
    pthread_t tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;
    
    param_struct* params=(param_struct*)malloc(sizeof(param_struct)*27);
    for (int i=0; i<9; i++) {
        params[i].starting_row=i;
        params[i].starting_col=i;
        pthread_create(&tid, NULL, checkRow, &(params[i]));
        pthread_join(tid, NULL);
        if (worker_validation==(int*)1) {
            }
            else {
                return 0;
            }
        pthread_create(&tid, NULL, checkCol, &(params[i]));
        pthread_join(tid, NULL);
        if (worker_validation==(int*)1) {
            }
            else {
                return 0;
            }
    }
    for (int a=2; a<9 && a>=2; a+=3) {
        for (int b=2; b<9 && b>=2; b+=3) {
            params[0].ending_row=a+1;
            params[0].ending_col=b+1;
            pthread_create(&tid, NULL, checkSquare, &(params[0]));
            pthread_join(tid, NULL);
            if (worker_validation==(int*)1) {
            }
            else {
                return 0;
            }
        }
    }
    return 1;
}
