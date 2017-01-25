#include "dijkstra.h"

void board_init();
void board_clear();
void board_print();
void board_print_bitboard();
void board_make_move(const char *move);

void setup_board(struct board *board, GList *moves);
void print_board(struct board *board);
void print_single_bitboard(u64 bitboard, enum piece type);
u64 has_piece_at(u64 bitboard, int pos);
void init_board(struct board *board);
void update_bitboard(int idx, struct board *board, u64 new_bitboard);
void check_castle(enum piece piece, struct board *board, u64 src, u64 dst);
void make_move(struct board *board, u64 src, u64 dst);
void make_move_from_uci_str(struct board *board, char *move);
void print_help(gchar **args);

#define vert_line "\n----+----+----+----+----+----+----+----+----+\n"
#define board_file_row "    |  a |  b |  c |  d |  e |  f |  g |  h"
