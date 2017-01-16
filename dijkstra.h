#ifndef DIJK_H
#define DIJK_H

typedef unsigned long long u64;

#define NAME "Dijkstra 0.1"
#define AUTHOR "Grimm Sleeper"

#define BAD_CMD_STR "Unknown Command, ignoring...\n"

#define BOARDSIZE 64
#define PIECE_TYPES 13

#define START_BP 0x00FF000000000000
#define START_BN 0x4200000000000000
#define START_BB 0x2400000000000000
#define START_BR 0x8100000000000000
#define START_BQ 0x1000000000000000
#define START_BK 0x0800000000000000

#define START_WP 0x000000000000FF00
#define START_WN 0x0000000000000042
#define START_WB 0x0000000000000024
#define START_WR 0x0000000000000081
#define START_WQ 0x0000000000000010
#define START_WK 0x0000000000000008

#define START_NONE 0x0000FFFFFFFF0000

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

struct bitboards {
  u64 none;
  u64 pawns[2];
  u64 knights[2];
  u64 bishops[2];
  u64 rooks[2];
  u64 queens[2];
  u64 kings[2];
};

struct board {
  struct bitboards bb;
  enum colors color;
};

#define streq(a,b) (strcmp(a,b) == 0)
#define pretty_string(idx) (piece_strings[idx])

#endif
