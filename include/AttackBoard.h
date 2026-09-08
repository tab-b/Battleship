#ifndef ATTACK_BOARD_H
#define ATTACK_BOARD_H
#include "Board.h"

typedef enum { NOT_TRIED = 0, HIT = 1, MISS = 2 } HitStatus;
typedef struct {
    int Rows;
    int Cols;
    HitStatus *cells;
} AttackBoard;

void initializeAttackBoard(AttackBoard *board);
void freeAttackBoard(AttackBoard *board);
/* Getter coordinates must be inside the board. */
HitStatus getAttackAtLocation(const AttackBoard *board, int row, int col);
int wasAttackAlreadyAttempted(const AttackBoard *board, int row, int col);
/* Returns 1 on success, 0 for invalid or already attempted coordinates. */
int updateAttackBoard(AttackBoard *board, int row, int col, HitStatus status);
#endif
