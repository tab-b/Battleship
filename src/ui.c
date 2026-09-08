#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "ui.h"

static const char *const shipString[] = {"  ", "DT", "SB", "CR", "BP", "CA", "X "};
static const char *const attackString[] = {"  ", "H ", "M "};

void displayMainMenu(const char *program) {
    printf("Usage:\n  %s             (single player)\n"
           "  %s <port>      (host multiplayer)\n"
           "  %s <ip> <port> (join multiplayer)\n", program, program, program);
}

static void printColumns(int cols) {
    printf("   ");
    for(int c = 0; c < cols; ++c) printf("%2d ", c);
    putchar('\n');
}

void printShipBoard(const ShipBoard *board) {
    puts("Ship board:");
    printColumns(board->Cols);
    for(int r = 0; r < board->Rows; ++r) {
        printf("%c: ", 'A' + r);
        for(int c = 0; c < board->Cols; ++c)
            printf("%s|", shipString[getShipAtLocation(board, r, c)]);
        putchar('\n');
    }
}

void printAttackBoard(const AttackBoard *board) {
    puts("Attack board:");
    printColumns(board->Cols);
    for(int r = 0; r < board->Rows; ++r) {
        printf("%c: ", 'A' + r);
        for(int c = 0; c < board->Cols; ++c)
            printf("%s|", attackString[getAttackAtLocation(board, r, c)]);
        putchar('\n');
    }
}

void UIPrintShipBoard(const Player *player) {
    printf("%s's ", player->name);
    printShipBoard(&player->shipboard);
}

void UIPrintAttackBoard(const Player *player) {
    printf("%s's ", player->name);
    printAttackBoard(&player->atkboard);
}

/* Parse A0-style coordinates without integer overflow or trailing junk. */
static GameStatus readCoordinates(int count, int *r1, int *c1, int *r2, int *c2) {
    char buffer[100];
    if(!fgets(buffer, sizeof(buffer), stdin)) return INPUT_QUIT;
    if(!strchr(buffer, '\n') && !feof(stdin)) {
        int ch;
        while((ch = getchar()) != '\n' && ch != EOF) {}
        return INPUT_ERROR;
    }
    const unsigned char *p = (const unsigned char *)buffer;
    while(isspace(*p)) ++p;
    if(*p == 'q' || *p == 'Q') return INPUT_QUIT;
    int rows[2], cols[2];
    for(int i = 0; i < count; ++i) {
        while(isspace(*p)) ++p;
        int letter = toupper(*p);
        if(letter < 'A' || letter >= 'A' + BOARD_SIZE) return INPUT_ERROR;
        ++p;
        if(*p < '0' || *p >= '0' + BOARD_SIZE) return INPUT_ERROR;
        rows[i] = letter - 'A';
        cols[i] = *p++ - '0';
        if(i + 1 < count && !isspace(*p)) return INPUT_ERROR;
    }
    while(isspace(*p)) ++p;
    if(*p) return INPUT_ERROR;
    *r1 = rows[0]; *c1 = cols[0];
    *r2 = rows[count - 1]; *c2 = cols[count - 1];
    return INPUT_VALID;
}

GameStatus promptForShipPlacement(Player *player) {
    printf("%s, place your ships. Enter q to quit.\n", player->name);
    for(int i = 0; i < SHIP_COUNT; ++i) {
        for(;;) {
            UIPrintShipBoard(player);
            printf("Place %s (length %d), %s: ", shipString[ships[i]], (int)ships[i],
                   ships[i] == DESTROYER ? "e.g. A0" : "start and end, e.g. A0 A4");
            fflush(stdout);
            int r1, c1, r2, c2;
            GameStatus status = readCoordinates(ships[i] == DESTROYER ? 1 : 2,
                                                &r1, &c1, &r2, &c2);
            if(status == INPUT_QUIT) return status;
            if(status == INPUT_VALID &&
                placeShip(&player->shipboard, r1, c1, r2, c2, ships[i])) break;
            puts("Invalid placement. Use an empty straight range of the correct length.");
        }
    }
    return INPUT_VALID;
}

GameStatus promptForAttack(const Player *attacker, int *row, int *col) {
    UIPrintShipBoard(attacker);
    UIPrintAttackBoard(attacker);
    for(;;) {
        printf("%s, attack (A0-J9, or q to quit): ", attacker->name);
        fflush(stdout);
        int r2, c2;
        GameStatus status = readCoordinates(1, row, col, &r2, &c2);
        if(status == INPUT_QUIT) return status;
        if(status == INPUT_VALID &&
            !wasAttackAlreadyAttempted(&attacker->atkboard, *row, *col)) return status;
        puts("Invalid or already attempted coordinate. Try again.");
    }
}

void ending(Player *winner, Player *loser) {
    ++winner->wins;
    ++loser->losses;
    printf("%s won! %s loses.\n", winner->name, loser->name);
    UIPrintShipBoard(winner);
}
