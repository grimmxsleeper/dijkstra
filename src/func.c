#include "stdio.h"
#include "dijkstra.h"
#include "func.h"

u64 coord_to_u64(char file, char rank) {
  int file_int, rank_int;
  bool invalid = false;
  file_int = file - 'a';
  if(file_int < 0 || file_int > 7) {
    printf("invalid file: %c\n", file);
    invalid = true;
  }
  rank_int = rank - '0';
  if(rank_int < 1 || rank_int > 8) {
    printf("invalid rank: %c\n", rank);
    invalid = true;
  }
  if(invalid) {
    return 0ULL;
  }
  return 1ULL << ((8 * rank_int - file_int) - 1);
}
