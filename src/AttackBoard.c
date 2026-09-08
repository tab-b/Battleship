#include <stdio.h>
#include <stdlib.h>
#include "AttackBoard.h"

void initializeAttackBoard(AttackBoard *board) {
    board->Rows = board->Cols = BOARD_SIZE;
    board->cells = calloc((size_t)board->Rows * board->Cols, sizeof(*board->cells));
    if(!board->cells) {
        perror("Attack board allocation failed");
        exit(EXIT_FAILURE);
    }
}

void freeAttackBoard(AttackBoard *board) {
    if(!board) return;
    free(board->cells);
    board->cells = NULL;
    board->Rows = board->Cols = 0;
}

HitStatus getAttackAtLocation(const AttackBoard *board, int row, int col) {
    return board->cells[row * board->Cols + col];
}

int wasAttackAlreadyAttempted(const AttackBoard *board, int row, int col) {
    return getAttackAtLocation(board, row, col) != NOT_TRIED;
}

int updateAttackBoard(AttackBoard *board, int row, int col, HitStatus status) {
    if(row < 0 || row >= board->Rows || col < 0 || col >= board->Cols) return 0;
    if(status != HIT && status != MISS) return 0;
    if(wasAttackAlreadyAttempted(board, row, col)) return 0;
    board->cells[row * board->Cols + col] = status;
    return 1;
}
