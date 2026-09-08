#ifndef UI_H
#define UI_H
#include "Player.h"

typedef enum {
    INPUT_ERROR = -1, INPUT_QUIT = 0, INPUT_VALID = 1, GAME_COMPLETE = 2
} GameStatus;

void displayMainMenu(const char *program);
void printShipBoard(const ShipBoard *board);
void printAttackBoard(const AttackBoard *board);
void UIPrintShipBoard(const Player *player);
void UIPrintAttackBoard(const Player *player);
GameStatus promptForShipPlacement(Player *player);
GameStatus promptForAttack(const Player *attacker, int *row, int *col);
void ending(Player *winner, Player *loser);
#endif
