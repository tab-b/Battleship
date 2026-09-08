# Battleship

A console Battleship game made for one of my CS classes — revamped into small modules using a `Player`,
`ShipBoard`, and `AttackBoard` structure.
Ship lengths remain 1, 2, 3, 4, and 5, as in the original game.

## Where things belong

| Files | Responsibility |
| --- | --- |
| `include/Board.h` | Shared board dimensions and number of ships |
| `ShipBoard.h / ShipBoard.c` | Ship storage, placement validation, hits, and loss detection |
| `AttackBoard.h / AttackBoard.c` | Track hit/miss results and repeated guesses |
| `Player.h / Player.c` | Own both boards, place a computer fleet, and resolve attacks between players |
| `ui.h / ui.c` | Console printing and coordinate input |
| `Game.h / Game.c` | Single-player and multiplayer turn sequences |
| `Network.h / Network.c` | TCP connection setup, complete message reads/writes, and closing sockets |
| `src/main.c` | Command-line arguments, random seed, and choosing the game mode |

Headers contain the public types and function declarations. Include a
module's header to use it; never include its `.c` file. Implementation-only
helpers and display strings are `static`, so they stay private to their
source file. The shared fleet array is declared `extern const` in its header
and defined once in `ShipBoard.c`.

Each `Player` owns its two board structs directly. Initialization allocates
only the cells, so `freePlayer()` frees those cells, not the player or the
embedded structs. This also avoids the conflicting `freeBoard()` functions
in the partial split: each board now has a distinct cleanup function.

Board cells use a flat array indexed by `row * Cols + col`. Public placement
and attack functions validate coordinates before indexing. The getter
functions expect valid coordinates, as documented in their headers.

## Build and play

Use Linux or WSL with a C11 compiler. The networking module uses POSIX sockets,
as the original did; native Windows builds need a Winsock port.

From this directory in a Linux/WSL shell:

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -O2 -Iinclude src/*.c -o battleship
./battleship
```

Or use `make` if installed. In PowerShell, prefix the compiler command with
`wsl --exec` and list the source files explicitly, or build from a WSL shell.

For multiplayer, run these in separate terminals:

```sh
./battleship 5000
./battleship 127.0.0.1 5000
```

Replace `127.0.0.1` with the host's IPv4 address to play across computers.
The host attacks first. Both players place their fleets before attacks begin.
Enter one coordinate for the destroyer (`A0`), two endpoints for longer ships
(`B0 B1`), and one coordinate per attack. Enter `q` or end input to quit.

## Changes made while splitting

- Fixed allocation into local pointer copies, excessive board loop bounds,
  and freeing embedded structs.
- Fixed name truncation, reversed occupancy checks, swapped row/column access,
  missing returns, and hits failing to mark ships destroyed.
- Completed placement and attack input, including overlap, malformed input,
  out-of-range coordinates, and repeated guesses.
- Kept quit status separate from a completed game.
- Removed global player boards and moved socket details out of the game rules.
- Replaced the threaded network loop with a placement handshake and alternating
  blocking sends/receives. Only one reader owns the socket, and complete TCP
  messages are assembled even when reads are partial. No pthread dependency
  remains.

The network loop reports a peer quitting when it next reads the socket; it
does not interrupt a local keyboard prompt. The wire format retains the
original native `int` message layout, so peers must use matching integer sizes
and byte order.

## Tests

With Python 3 and a GCC-compatible compiler in Linux/WSL:

```sh
python3 tests/run_tests.py
```

From PowerShell:

```powershell
wsl --exec python3 tests/run_tests.py
```

This builds with warnings treated as errors, AddressSanitizer, and
UndefinedBehaviorSanitizer, then checks board boundaries, overlap, reversed
placement, hit/miss tracking, cleanup, 500 randomized fleets, exhausted attack
boards, CLI validation, a complete single-player game, and two-process
multiplayer victory and quit paths. `make test` runs the tests with the normal
build; `make check` runs the sanitizer suite.
