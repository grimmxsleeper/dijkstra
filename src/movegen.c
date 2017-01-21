#include <glib.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "masks.h"
#include "movegen.h"
#include "func.h"
#include "board.h"
#include "bitboards.h"

void generate_moves(struct board *board) {
  if(board->possible_moves) {
    g_list_free_full(board->possible_moves, g_free);
    board->possible_moves = NULL;
  }
  piece_per_color_loop(ix) {
    if(board->color) {
      gen_moves_piece(board, black_pieces[ix]);
    } else {
      gen_moves_piece(board, white_pieces[ix]);
    }
  }
  print_moves_u64(board);
}

// void gen_moves_pawn(struct board *board) {
//
// }
void gen_moves_piece(struct board *board, enum piece type) {
  u64 bitboard, src, dst;
  u64 *bb_move;
  switch(type) {
    case WP:
      bitboard = board->bitboards[type];
      board_loop(ix) {
        if(has_piece_at(bitboard, ix)) {

          // move forward 1 square
          src = pos_to_u64(ix);
          dst = src << 8ULL;
          if(!piece_type_at_u64(board, dst)) {
            if(is_valid_move(board, type,
               piece_type_at_u64(board, dst), src, dst)) {
              bb_move = calloc(2, sizeof(u64));
              bb_move[0] = src;
              bb_move[1] = dst;
              board->possible_moves = g_list_prepend(
                board->possible_moves, bb_move);
            }
          }

          // only moves up 2 squares if in the starting position
          if(pos_to_u64(ix) & START_WP) {
            src = pos_to_u64(ix);
            dst = src << 16ULL;

            // pawn only moves forward in non-attack
            if(!piece_type_at_u64(board, dst)) {
              if(is_valid_move(board, type,
                               piece_type_at_u64(board, dst), src, dst)) {
                bb_move = calloc(2, sizeof(u64));
                bb_move[0] = src;
                bb_move[1] = dst;
                board->possible_moves = g_list_prepend(
                  board->possible_moves, bb_move);
              }
            }
          }
        }
      }
      break;
    case BP:
      bitboard = board->bitboards[type];
      board_loop(ix) {
        if(has_piece_at(bitboard, ix)) {

          // move forward 1 square
          src = pos_to_u64(ix);
          dst = src >> 8ULL;
          if(!piece_type_at_u64(board, dst)) {
            if(is_valid_move(board, type,
                             piece_type_at_u64(board, dst), src, dst)) {
              bb_move = calloc(2, sizeof(u64));
              bb_move[0] = src;
              bb_move[1] = dst;
              board->possible_moves = g_list_prepend(
                board->possible_moves, bb_move);
            }
          }

          // only moves up 2 squares if in the starting position
          if(pos_to_u64(ix) & START_BP) {
            src = pos_to_u64(ix);
            dst = src >> 16ULL;

            // pawn only moves forward in non-attack
            if(!piece_type_at_u64(board, dst)) {
              if(is_valid_move(board, type,
                               piece_type_at_u64(board, dst), src, dst)) {
                bb_move = calloc(2, sizeof(u64));
                bb_move[0] = src;
                bb_move[1] = dst;
                board->possible_moves = g_list_prepend(
                  board->possible_moves, bb_move);
              }
            }
          }
        }
      }
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
