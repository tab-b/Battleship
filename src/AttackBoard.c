#include <stdio.h>
#include <math.h>
#include "AttackBoard.h"

void initializeAttackBoard(AttackBoard* board) {
    board = malloc(sizeof(AttackBoard));
    if(!board) {
        perror("Allocation failed");
        exit(1);
    }
    board->Rows = BOARD_SIZE;
    board->Cols = BOARD_SIZE;
    board->cells = malloc(board->Rows * board->Cols * sizeof(HitStatus));

    if(!board->cells) {
        perror("Cell allocation failed!");
        free(board);
        exit(1);
    }

    for(int i = 0; i < pow(BOARD_SIZE, BOARD_SIZE); i++) {
        board->cells[i] = NOT_TRIED;
    }
    return board;
}

static inline int getIndex(AttackBoard* board, int row, int col) {
    return row * board->Cols + col;
}

HitStatus getAttackAtLocation(AttackBoard* board, int row, int col) {
    return board->cells[getIndex(board, row, col)];
}

void printAttackBoard(AttackBoard* board) {
    printf("Attack board:\n");
    printf("   ");
    for(int col = 0; col < BOARD_SIZE; col++) printf("%2d ", col);
    printf("\n");
    for(int i = 0; i < BOARD_SIZE; i++) {
        printf("%c: ", 'A' + i);
        for(int j = 0; j < BOARD_SIZE; j++) {
            printf("%s|", attackString[getAttackAtLocation(board, i, j)]);
        }
    }
}

int wasAttackAlreadyAttempted(AttackBoard* board, int row, int col) {
    HitStatus attack = getAttackAtLocation(board, row, col);
    return (attack != NOT_TRIED); // if attack doesn't equal not tried, it was tried. 
}

int updateAttackBoard(AttackBoard* board, int row, int col, HitStatus status) {
    if(wasAttackAlreadyAttempted(board, row, col) == 1) return 0; // location already attempted
    int location = getIndex(board, row, col);
    board->cells[location] = status;

}


void freeBoard(AttackBoard* board) {
    if(!board) return;
    free(board->cells);
    free(board);
}