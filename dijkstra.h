#ifndef DIJK_H
#define DIJK_H

typedef unsigned long long u64;

#define NAME "Dijkstra 0.1"
#define AUTHOR "Grimm Sleeper"

#define BAD_CMD_STR "Unknown Command, ignoring...\n"

#define BOARDSIZE 64
#define BOARDLEN 8
#define PIECE_TYPES 13
#define NUM_DIAG 2 * BOARDLEN - 1

enum colors {
  WHITE = 0,
  BLACK = 1
};

enum piece {
  NONE = 0,
  WP = 1,
  WN = 2,
  WB = 3,
  WR = 4,
  WQ = 5,
  WK = 6,
  BP = 7,
  BN = 8,
  BB = 9,
  BR = 10,
  BQ = 11,
  BK = 12,
};

extern const char *piece_strings[PIECE_TYPES];

struct board {
  u64 bitboards[PIECE_TYPES];
  enum colors color;
};

#define streq(a,b) (strcmp(a,b) == 0)
#define pretty_string(idx) (piece_strings[idx])
#define print_pretty_string(idx) (printf("%s", pretty_string(idx)))
#define print_pretty_string_newline(idx) (printf("%s\n", pretty_string(idx)))
#define piece_loop(ix) for(int ix = 0; ix < PIECE_TYPES; ix++)
#define board_loop(ix) for(ix = BOARDSIZE - 1; ix >= 0; ix--)
#define boardlen_loop(ix) for(int ix = 0; ix < BOARDLEN; ix++)
#define diag_num_loop(ix) for(int ix = 0; ix < NUM_DIAG; ix++)

typedef enum { false, true } bool;

#endif
