CC = cc
CPPFLAGS = -Iinclude
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
SOURCES = src/main.c src/Game.c src/Network.c src/ui.c src/Player.c src/ShipBoard.c src/AttackBoard.c
HEADERS = $(wildcard include/*.h)

.PHONY: all test check clean
all: battleship

battleship: $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@

test: battleship tests/test_boards
	./tests/test_boards
	python3 tests/test_game.py ./battleship

check:
	python3 tests/run_tests.py

tests/test_boards: tests/test_boards.c src/Player.c src/ShipBoard.c src/AttackBoard.c $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_boards.c src/Player.c src/ShipBoard.c src/AttackBoard.c -o $@

clean:
	rm -f battleship tests/test_boards
