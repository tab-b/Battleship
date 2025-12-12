#ifndef AttackBoard_H
#define AttackBoard_H
#include <stddef.h>

#define BOARD_SIZE 10

typedef enum {
    NOT_TRIED = 0,
    HIT = 1,
    MISS = 2
} HitStatus;

typedef struct {
    int Rows;
    int Cols;
    HitStatus* cells;
} AttackBoard;

HitStatus* initializeAttackBoard(void);
void printAttackBoard(AttackBoard* atkboard);
int wasAttackAlreadyAttempted(AttackBoard* atkboard, char row, int col);
int updateAttackBoard(AttackBoard* atkboard, char row, int col, HitStatus status);

#endif