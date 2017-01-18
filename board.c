#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "dijkstra.h"
#include "board.h"
#include "func.h"
#include "bitboards.h"

/**
 * For now, this will start from the standard board setup
 *
 * TODO: Add FEN param
 */
void setup_board(struct board *board, GList *moves) {
  if(moves) {
    if(!streq(moves->data, "moves")) {
      printf(BAD_CMD_STR);
      return;
    }
  }
  init_board(board);
  if(moves) {
    GList *list;
    char *move;
    int ix;
    u64 src, dst, new_bitboard;
    for (list = moves->next; list != NULL; list = list->next){
      move = list->data;
      if(strlen(move) != 4) {
        printf(BAD_CMD_STR);
        return;
      }
      src = coord_to_u64(move[0], move[1]);
      dst = coord_to_u64(move[2], move[3]);
      if(!src || !dst) {
        printf("invalid move: %s\n", move);
        return;
      }
      piece_loop(ix) {
        if(board->bitboards[ix] & src) {
          if(ix == NONE) {
            printf("invalid move: %s\n", move);
            return;
          }
          // remove dst piece if captured
          piece_loop(jx) {
            if(board->bitboards[jx] & dst) {
              pretty_string(jx);
              new_bitboard = (board->bitboards[jx] ^ dst);
              update_bitboard(jx, board, new_bitboard);
              break;
            }
          }

          // always move piece from dst to src
          new_bitboard = (board->bitboards[ix] ^ src) | dst;
          update_bitboard(ix, board, new_bitboard);

          // always add NONE to src of move
          new_bitboard = (board->bitboards[NONE]) | src;
          update_bitboard(NONE, board, new_bitboard);
          break;
        }
      }
    }
  }
}

void print_board(struct board *board) {
  if(board) {
    int ix, jx;
    printf("  |  a |  b |  c |  d |  e |  f |  g |  h\n");
    printf("-----------------------------------------");
    board_loop(ix){
      if((ix + 1) % 8 == 0) {
        printf("\n%d | ", (ix + 1) / 8);
      }
      piece_loop(jx) {
        if(has_piece_at(board->bitboards[jx], ix)) {
          printf("%s | ", pretty_string(jx));
          break;
        }
      }
    }
    printf("\n");
  } else {
    printf("board is NULL!\n");
  }
}

void print_single_bitboard(u64 bitboard) {
  int ix, jx, found;
  printf("  |  a |  b |  c |  d |  e |  f |  g |  h\n");
  printf("-----------------------------------------");
  board_loop(ix) {
    found = 0;
    if((ix + 1) % 8 == 0) {
      printf("\n%d | ", (ix + 1) / 8);
    }
    piece_loop(jx) {
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
    board->bitboards[NONE] = START_NONE;
    board->bitboards[WP] = START_WP;
    board->bitboards[WN] = START_WN;
    board->bitboards[WB] = START_WB;
    board->bitboards[WR] = START_WR;
    board->bitboards[WQ] = START_WQ;
    board->bitboards[WK] = START_WK;
    board->bitboards[BP] = START_BP;
    board->bitboards[BN] = START_BN;
    board->bitboards[BB] = START_BB;
    board->bitboards[BR] = START_BR;
    board->bitboards[BQ] = START_BQ;
    board->bitboards[BK] = START_BK;
  } else {
    printf("board is NULL!\n");
  }
}

void update_bitboard(int idx, struct board *board, u64 new_bitboard) {
  if(board) {
    board->bitboards[idx] = new_bitboard;
  } else {
    printf("board is NULL!\n");
  }
}
