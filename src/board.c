#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "dijkstra.h"
#include "board.h"
#include "func.h"
#include "bitboards.h"
#include "masks.h"

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
    for (list = moves->next; list != NULL; list = list->next){
      move = list->data;
      make_move_from_uci_str(board, move);
    }
  }
}

void make_move_from_uci_str(struct board *board, char *move) {
  if(strlen(move) != 4) {
    print_invalid_move(move);
    return;
  }
  u64 src, dst;
  src = coord_to_u64(move[0], move[1]);
  dst = coord_to_u64(move[2], move[3]);
  if(!src || !dst) {
    print_invalid_move(move);
  } else {
    make_move(board, src, dst);
  }
}

void make_move(struct board *board, u64 src, u64 dst) {
  char src_str[3], dst_str[3];
  u64_to_coord(src, src_str, sizeof(src_str));
  u64_to_coord(dst, dst_str, sizeof(dst_str));
  if(src == dst) {
    print_invalid_move_split(src_str, dst_str);
    return;
  }
  u64 new_bitboard;
  piece_loop(ix) {
    if(board->bitboards[ix] & src) {
      if(ix == NONE) {
        print_invalid_move_split(src_str, dst_str);
        return;
      }
      // remove dst piece if captured
      piece_loop(jx) {
        if(board->bitboards[jx] & dst) {
          new_bitboard = (board->bitboards[jx] ^ dst);
          update_bitboard(jx, board, new_bitboard);
          break;
        }
      }

      // always move piece from src to dst
      new_bitboard = (board->bitboards[ix] ^ src) | dst;
      update_bitboard(ix, board, new_bitboard);

      // always add NONE to src of move
      new_bitboard = (board->bitboards[NONE]) | src;
      update_bitboard(NONE, board, new_bitboard);

      check_castle(ix, board, src, dst);
      break;
    }
  }
}

void check_castle(enum piece piece, struct board *board, u64 src, u64 dst) {
  u64 new_bitboard;
  if(piece == WK) {
    if(src & START_WK) {
      if(dst & WK_CASTLE_QUEEN) {
        if(board->WK_castle_queen) {
          if(board->bitboards[WR] & WR_QUEEN) {
            new_bitboard = (board->bitboards[WR] ^ WR_QUEEN) | WR_CASTLE_QUEEN;
            update_bitboard(WR, board, new_bitboard);
            new_bitboard = (board->bitboards[NONE] ^ WR_CASTLE_QUEEN) | BR_QUEEN;
            update_bitboard(NONE, board, new_bitboard);
          }
        }
      } else if(dst & WK_CASTLE_KING) {
        if(board->WK_castle_king) {
          if(board->bitboards[WR] & WR_KING) {
            new_bitboard = (board->bitboards[WR] ^ WR_KING) | WR_CASTLE_KING;
            update_bitboard(WR, board, new_bitboard);
            new_bitboard = (board->bitboards[NONE] ^ WR_CASTLE_KING) | WR_KING;
            update_bitboard(NONE, board, new_bitboard);
          }
        }
      }
    }
    // after king has moved set castling to false
    board->WK_castle_queen = false;
    board->WK_castle_king = false;
  } else if(piece == BK) {
    if(src & START_BK) {
      if(dst & BK_CASTLE_QUEEN) {
        if(board->BK_castle_queen) {
          if(board->bitboards[BR] & BR_QUEEN) {
            new_bitboard = (board->bitboards[BR] ^ BR_QUEEN) | BR_CASTLE_QUEEN;
            update_bitboard(BR, board, new_bitboard);
            new_bitboard = (board->bitboards[NONE] ^ BR_QUEEN) | BR_QUEEN;
            update_bitboard(NONE, board, new_bitboard);
          }
        }
      } else if(dst & BK_CASTLE_KING) {
        if(board->BK_castle_king) {
          if(board->bitboards[BR] & BR_KING) {
            new_bitboard = (board->bitboards[BR] ^ BR_KING) | BR_CASTLE_KING;
            update_bitboard(BR, board, new_bitboard);
            new_bitboard = (board->bitboards[NONE] ^ BR_CASTLE_KING) | BR_KING;
            update_bitboard(NONE, board, new_bitboard);
          }
        }
      }
    }
    // after king has moved set castling to false
    board->BK_castle_queen = false;
    board->BK_castle_king = false;
  }
}

void print_board(struct board *board) {
  if(board) {
    printf(board_file_row);
    board_loop(ix){
      if((ix + 1) % 8 == 0) {
        printf(vert_line);
        printf("  %d | ", (ix + 1) / 8);
      }
      piece_loop(jx) {
        if(has_piece_at(board->bitboards[jx], ix)) {
          printf("%s | ", piece_strings[jx]);
          break;
        }
      }
    }
    printf("%s\n", vert_line);
  } else {
    printf("board is NULL!\n");
  }
}

void print_single_bitboard(u64 bitboard, enum piece type) {
  printf(board_file_row);
  board_loop(ix) {
    if((ix + 1) % 8 == 0) {
      printf(vert_line);
      printf("  %d | ", (ix + 1) / 8);
    }
    if(has_piece_at(bitboard, ix)) {
      if(type == NONE) {
        printf("xx | ");
      } else {
        printf("%s | ", piece_strings[type]);
      }
    } else {
      printf("   | ");
    }
  }
  printf("%s\n", vert_line);
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
    board->WK_castle_queen = true;
    board->WK_castle_king = true;
    board->BK_castle_queen = true;
    board->BK_castle_king = true;
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
