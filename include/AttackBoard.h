#ifndef AttackBoard_H
#define AttackBoard_H
#include <stddef.h>

#define BOARD_SIZE 10

const char *attackString[] = {"  ", "H ", "M "}; 

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

void initializeAttackBoard(AttackBoard* board);
void printAttackBoard(AttackBoard* board);
int wasAttackAlreadyAttempted(AttackBoard* board, int row, int col);
int updateAttackBoard(AttackBoard* atkboard, int row, int col, HitStatus status);
void freeBoard(AttackBoard* board);

#endif