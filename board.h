#include "dijkstra.h"

void setup_board(struct board *board, GList *moves);
void print_board(struct board *board);
void print_single_bitboard(u64 bitboard);
u64 has_piece_at(u64 bitboard, int pos);
void init_board(struct board *board);
void update_bitboard(int idx, struct board *board, u64 new_bitboard);
