#ifndef UI_H
#define UI_H

#include "Player.h"

void displayMainMenu();

void printShipBoard(Player* player);
void printAttackBoard(Player* player);

void promptForShipPlacement(Player* player);
void promptForAttack(Player* attacker, Player* defender);

void ending(Player* winner, Player* loser);

#endif