#include <stdio.h>
#include "Game.h"
#include "Network.h"

GameStatus playSinglePlayer(void) {
    Player human, computer;
    initializePlayer(&human, "Player 1");
    initializePlayer(&computer, "Computer");
    autoPlacement(&computer);
    GameStatus status = promptForShipPlacement(&human);
    while(status == INPUT_VALID) {
        int row, col;
        status = promptForAttack(&human, &row, &col);
        if(status != INPUT_VALID) break;
        puts(attackPlayer(&human, &computer, row, col) ? "HIT!" : "MISS!");
        if(hasPlayerLost(&computer)) {
            ending(&human, &computer);
            status = GAME_COMPLETE;
            break;
        }
        int hit = computerAttack(&computer, &human, &row, &col);
        printf("Computer attacks at %c%d: %s\n", 'A' + row, col, hit ? "HIT!" : "MISS!");
        if(hasPlayerLost(&human)) {
            ending(&computer, &human);
            status = GAME_COMPLETE;
        }
    }
    freePlayer(&human);
    freePlayer(&computer);
    return status;
}

static int isControl(const MoveMsg *move, int code) {
    return move->row == code && move->col == code;
}

static int sendControl(int socketFd, int code) {
    MoveMsg move = {code, code, 0};
    return sendMove(socketFd, &move);
}

static GameStatus waitForMove(int socketFd, MoveMsg *move) {
    if(!receiveMove(socketFd, move)) {
        puts("Other player disconnected.");
        return INPUT_QUIT;
    }
    if(isControl(move, MSG_QUIT)) {
        puts("Other player quit.");
        return INPUT_QUIT;
    }
    return INPUT_VALID;
}

GameStatus playMultiplayer(int socketFd, int isServer) {
    Player local;
    initializePlayer(&local, isServer ? "Player 1" : "Player 2");
    GameStatus status = promptForShipPlacement(&local);
    MoveMsg move;
    if(status != INPUT_VALID) {
        sendControl(socketFd, MSG_QUIT);
        freePlayer(&local);
        return status;
    }
    if(!sendControl(socketFd, MSG_DONE)) {
        freePlayer(&local);
        return INPUT_ERROR;
    }
    puts("Waiting for other player to finish placement...");
    status = waitForMove(socketFd, &move);
    if(status == INPUT_VALID && !isControl(&move, MSG_DONE)) status = INPUT_ERROR;
    int myTurn = isServer;
    while(status == INPUT_VALID) {
        if(myTurn) {
            int row, col;
            status = promptForAttack(&local, &row, &col);
            if(status != INPUT_VALID) {
                sendControl(socketFd, MSG_QUIT);
                break;
            }
            MoveMsg attack = {row, col, 0};
            if(!sendMove(socketFd, &attack)) { status = INPUT_ERROR; break; }
            status = waitForMove(socketFd, &move);
            if(status != INPUT_VALID) break;
            if(isControl(&move, MSG_WIN)) {
                updateAttackBoard(&local.atkboard, row, col, HIT);
                puts("You won! Enemy's last ship has been destroyed.");
                status = GAME_COMPLETE;
                break;
            }
            if(move.row != row || move.col != col ||
                (move.result != 0 && move.result != 1)) {
                status = INPUT_ERROR;
                break;
            }
            updateAttackBoard(&local.atkboard, row, col, move.result ? HIT : MISS);
            puts(move.result ? "HIT!" : "MISS!");
        } else {
            puts("Waiting for other player's attack...");
            status = waitForMove(socketFd, &move);
            if(status != INPUT_VALID) break;
            int hit = attackShip(&local.shipboard, move.row, move.col);
            if(hit < 0) { status = INPUT_ERROR; break; }
            printf("They attack at %c%d: %s\n", 'A' + move.row, move.col,
                   hit ? "HIT!" : "MISS!");
            if(hasPlayerLost(&local)) {
                puts("All your ships are destroyed! You lost.");
                status = sendControl(socketFd, MSG_WIN) ? GAME_COMPLETE : INPUT_ERROR;
                break;
            }
            move.result = hit;
            if(!sendMove(socketFd, &move)) { status = INPUT_ERROR; break; }
        }
        myTurn = !myTurn;
    }
    freePlayer(&local);
    return status;
}
