#include "stdio.h"
#include "dijkstra.h"
#include "func.h"
#include "masks.h"

u64 coord_to_u64(char file, char rank) {
  int file_int, rank_int;
  bool invalid = false;
  file_int = file - 'a';
  if(file_int < 0 || file_int > 7) {
    invalid = true;
  }
  rank_int = rank - '0';
  if(rank_int < 1 || rank_int > 8) {
    invalid = true;
  }
  if(invalid) {
    return 0ULL;
  }
  return 1ULL << ((8 * rank_int - file_int) - 1);
}

void u64_to_coord(u64 pos, char *str, int size) {
  int file_int, rank_int;
    boardlen_loop(ix) {
      if(ranks[ix] & pos) {
        rank_int = ix;
      }
      if(files[ix] & pos) {
        file_int = ix;
      }
    }
  snprintf(str, size, "%c%c", 'a' + file_int, '1' + rank_int);
}

enum color color(enum piece piece) {
  if(!piece) {
    return -1;
  }
  for(int ix = 0; ix < PIECE_PER_COLOR; ix++) {
    if(piece == white_pieces[ix]) {
      return WHITE;
    }
  }
  return BLACK;
}
