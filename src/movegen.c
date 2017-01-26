#include <glib.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "masks.h"
#include "movegen.h"
#include "func.h"
#include "board.h"
#include "bitboards.h"

// Global board
struct board board;

void generate_moves() {
  if(board.possible_moves) {
    g_list_free_full(board.possible_moves, g_free);
    board.possible_moves = NULL;
  }
  piece_per_color_loop(ix) {
    if(board.color) {
      gen_moves_piece(black_pieces[ix]);
    } else {
      gen_moves_piece(white_pieces[ix]);
    }
  }
  print_moves_u64(&board);
}

void _gen_moves_pawn_white() {
  u64 bitboard, src, dst;
  u64 *bb_move;
  enum piece type = WP;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {

      // move forward 1 square
      src = pos_to_u64(ix);
      dst = src << 8ULL;
      if(!piece_type_at_u64(&board, dst)) {
        if(is_valid_move(&board, type,
           piece_type_at_u64(&board, dst), src, dst)) {
          bb_move = calloc(2, sizeof(u64));
          bb_move[0] = src;
          bb_move[1] = dst;
          board.possible_moves = g_list_prepend(
            board.possible_moves, bb_move);
        }
      }

      // only moves up 2 squares if in the starting position
      if(pos_to_u64(ix) & START_WP) {
        src = pos_to_u64(ix);
        dst = src << 16ULL;

        // pawn only moves forward in non-attack
        if(!piece_type_at_u64(&board, dst)) {
          if(is_valid_move(&board, type,
                           piece_type_at_u64(&board, dst), src, dst)) {
            bb_move = calloc(2, sizeof(u64));
            bb_move[0] = src;
            bb_move[1] = dst;
            board.possible_moves = g_list_prepend(
              board.possible_moves, bb_move);
          }
        }
      }
    }
  }
}

void _gen_moves_pawn_black() {
  u64 bitboard, src, dst;
  u64 *bb_move;
  enum piece type = BP;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {

      // move forward 1 square
      src = pos_to_u64(ix);
      dst = src >> 8ULL;
      if(!piece_type_at_u64(&board, dst)) {
        if(is_valid_move(&board, type,
                         piece_type_at_u64(&board, dst), src, dst)) {
          bb_move = calloc(2, sizeof(u64));
          bb_move[0] = src;
          bb_move[1] = dst;
          board.possible_moves = g_list_prepend(
            board.possible_moves, bb_move);
        }
      }

      // only moves up 2 squares if in the starting position
      if(pos_to_u64(ix) & START_BP) {
        src = pos_to_u64(ix);
        dst = src >> 16ULL;

        // pawn only moves forward in non-attack
        if(!piece_type_at_u64(&board, dst)) {
          if(is_valid_move(&board, type,
                           piece_type_at_u64(&board, dst), src, dst)) {
            bb_move = calloc(2, sizeof(u64));
            bb_move[0] = src;
            bb_move[1] = dst;
            board.possible_moves = g_list_prepend(
              board.possible_moves, bb_move);
          }
        }
      }
    }
  }
}

void gen_moves_piece(enum piece type) {
  switch(type) {
    case WP:
      _gen_moves_pawn_white();
      break;
    case BP:
      _gen_moves_pawn_white();
      break;
    case WN:
    case BN:
      break;
    case WB:
    case BB:
      break;
    case WR:
    case BR:
      break;
    case WK:
    case BK:
      break;
    case WQ:
    case BQ:
      break;
    case NONE:
    default:
      break;
  }

}
