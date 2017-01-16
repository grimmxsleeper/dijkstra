#include <glib.h>
#include <stdio.h>
#include "dijkstra.h"
#include "board.h"

/**
 * For now, this will start from the standard board setup
 *
 * TODO: Add FEN param
 */
void setup_board(struct board *board, GList *moves) {
  init_board(board);
  // make moves
}

void print_board(struct board *board) {
  if(board) {
    int ix, jx;
    u64 bitboards[PIECE_TYPES];
    get_bitboards(board, bitboards);
    for(jx = 0; jx < PIECE_TYPES; jx++) {
      printf("%s\n", pretty_string(jx));
      print_single_bitboard(bitboards[jx]);
    }
    printf("  |  a |  b |  c |  d |  e |  f |  g |  h\n");
    printf("-----------------------------------------");
    for(ix = BOARDSIZE - 1; ix >= 0; ix--) {
      if((ix + 1) % 8 == 0) {
        printf("\n%d | ", (ix + 1) / 8);
      }
      for(jx = 0; jx < PIECE_TYPES; jx++) {
        if(has_piece_at(bitboards[jx], ix)) {
          printf("%s | ", pretty_string(jx));
          break;
        }
      }
    }
  } else {
    printf("board is NULL!");
  }
}

void print_single_bitboard(u64 bitboard) {
  int ix, jx, found;
  printf("  |  a |  b |  c |  d |  e |  f |  g |  h\n");
  printf("-----------------------------------------");
  for(ix = BOARDSIZE - 1; ix >= 0; ix--) {
    found = 0;
    if((ix + 1) % 8 == 0) {
      printf("\n%d | ", (ix + 1) / 8);
    }
    for(jx = 0; jx < PIECE_TYPES; jx++) {
      if(has_piece_at(bitboard, ix)) {
        printf("%s | ", pretty_string(jx));
        found = 1;
        break;
      }
    }
    if(!found) {
      printf("00 | ");
    }
  }
  printf("\n");
}

u64 has_piece_at(u64 bitboard, int pos) {
  return bitboard & (1ULL << pos);
}

void init_board(struct board *board) {
  if(board) {
    board->bb.none = START_NONE;
    board->bb.pawns[WHITE] = START_WP;
    board->bb.pawns[BLACK] = START_BP;
    board->bb.knights[WHITE] = START_WN;
    board->bb.knights[BLACK] = START_BN;
    board->bb.bishops[WHITE] = START_WB;
    board->bb.bishops[BLACK] = START_BB;
    board->bb.rooks[WHITE] = START_WR;
    board->bb.rooks[BLACK] = START_BR;
    board->bb.queens[WHITE] = START_WQ;
    board->bb.queens[BLACK] = START_BQ;
    board->bb.kings[WHITE] = START_WK;
    board->bb.kings[BLACK] = START_BK;
  } else {
    printf("board is NULL!");
  }
}

void get_bitboards(struct board *board, u64 bitboards[PIECE_TYPES]) {
  if(board) {
    bitboards[NONE] = board->bb.none;
    bitboards[WP] = board->bb.pawns[WHITE];
    bitboards[BP] = board->bb.pawns[BLACK];
    bitboards[WN] = board->bb.knights[WHITE];
    bitboards[BN] = board->bb.knights[BLACK];
    bitboards[WB] = board->bb.bishops[WHITE];
    bitboards[BB] = board->bb.bishops[BLACK];
    bitboards[WR] = board->bb.rooks[WHITE];
    bitboards[BR] = board->bb.rooks[BLACK];
    bitboards[WQ] = board->bb.queens[WHITE];
    bitboards[BQ] = board->bb.queens[BLACK];
    bitboards[WK] = board->bb.kings[WHITE];
    bitboards[BK] = board->bb.kings[BLACK];
  } else {
    printf("board is NULL!");
  }
}
