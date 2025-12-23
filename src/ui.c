#include <stdio.h>
#include "ui.h"
#include "ShipBoard.h"
#include "AttackBoard.h"


void displayMainMenu();

void UIPrintShipBoard(Player* player) {
    printf("%s's ship board: \n", player->name);
    printShipBoard(&player->shipboard);

}
void UIPrintAttackBoard(Player* player) {
    printf("%s's attack board: ", player->name);
    printAttackBoard(&player->atkboard);
}

void promptForShipPlacement(Player* player) {
    printf("%s, it is time for you to place your ship!", player->name);
}
void promptForAttack(Player* attacker, Player* defender) {
    printf("%s, attack! Type a letter for a row (A-J) followed by a number (0-9) without a space!")
}

void ending(Player* winner, Player* loser) {
    printf("%s won! %s loses.", winner->name, loser->name);
}