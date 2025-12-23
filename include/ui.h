#ifndef UI_H
#define UI_H

#include "Player.h"
#include "ShipBoard.h"
#include "AttackBoard.h"

void displayMainMenu();

void UIPrintShipBoard(Player* player);
void UIPrintAttackBoard(Player* player);

void promptForShipPlacement(Player* player);
void promptForAttack(Player* attacker, Player* defender);

void ending(Player* winner, Player* loser);

#endif