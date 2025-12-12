#include <stdio.h>
#include <math.h>
#include "AttackBoard.h"

AttackBoard* initializeAttackBoard(void) {
    AttackBoard* board = malloc(sizeof(AttackBoard));
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