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

/**
 * Generate all possible moves for a given board position
 */
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
}

/**
 * Add a move to the current possible moves if the move is acceptable
 *
 * @param src Position to move from in u64
 * @param dst Position to move to in u64
 * @param path The path to move along (diagonal/file/rank mask) or 0
 * @param type The type of piece to add move for
 */
static void _add_move_if_valid(u64 src, u64 dst, u64 path, enum piece type) {
  u64 *bb_move;
  if(is_valid_move(&board, type,
     piece_type_at_u64(&board, dst), src, dst, path)) {

    bb_move = calloc(2, sizeof(u64));
    bb_move[0] = src;
    bb_move[1] = dst;
    board.possible_moves = g_list_prepend(
      board.possible_moves, bb_move);
  }
}


/**
 * Generate moves along a diagonal
 *
 * @param src Position to move from in u64
 * @param bitboard Current bitboard for the given piece
 * @param type The type of piece to generate moves for
 */
static void _moves_diagonal(u64 src, u64 bitboard, enum piece type) {
  u64 dst;
  diag_num_loop(jx) {
    if(diags_r[jx] & src) {
      board_loop(kx) {
        if(diags_r[jx] & pos_to_u64(kx)) {
          dst = pos_to_u64(kx);
          _add_move_if_valid(src, dst, diags_r[jx], type);
        }
      }
    }
    if(diags_l[jx] & src) {
      board_loop(kx) {
        if(diags_l[jx] & pos_to_u64(kx)) {
          dst = pos_to_u64(kx);
          _add_move_if_valid(src, dst, diags_l[jx], type);
        }
      }
    }
  }
}

/**
 * Generate moves along a rank/file
 *
 * @param src Position to move from in u64
 * @param bitboard Current bitboard for the given piece
 * @param type The type of piece to generate moves for
 */
static void _moves_rank_file(u64 src, u64 bitboard, enum piece type) {
  u64 dst;
  boardlen_loop(jx) {
    if(ranks[jx] & src) {
      board_loop(kx) {
        if(ranks[jx] & pos_to_u64(kx)) {
          dst = pos_to_u64(kx);
          _add_move_if_valid(src, dst, ranks[jx], type);
        }
      }
    }
    if(files[jx] & src) {
      board_loop(kx) {
        if(files[jx] & pos_to_u64(kx)) {
          dst = pos_to_u64(kx);
          _add_move_if_valid(src, dst, files[jx], type);
        }
      }
    }
  }
}

/**
 * Generate moves for white pawns
 *
 * @param type The type of piece to generate moves for
 */
static void _gen_moves_pawn_white() {
  u64 bitboard, src, dst;
  enum piece type = WP;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {

      // move forward 1 square
      src = pos_to_u64(ix);
      dst = src << 8ULL;
      if(!piece_type_at_u64(&board, dst)) {
        _add_move_if_valid(src, dst, 0, type);
      }
      // only moves up 2 squares if in the starting position
      if(pos_to_u64(ix) & START_WP) {
        dst = src << 16ULL;

        // pawn only moves forward in non-attack
        if(!piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      }

      // check attack positions
      if(files[0] & src) {
        dst = src << 7ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      } else if(files[BOARDLEN - 1] & src) {
        dst = src << 9ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      } else {
        dst = src << 7ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
        dst = src << 9ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      }
    }
  }
}

/**
 * Generate moves for black pawns
 *
 * @param type The type of piece to generate moves for
 */
static void _gen_moves_pawn_black() {
  u64 bitboard, src, dst;
  enum piece type = BP;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {

      // move forward 1 square
      src = pos_to_u64(ix);
      dst = src >> 8ULL;
      if(!piece_type_at_u64(&board, dst)) {
        _add_move_if_valid(src, dst, 0, type);
      }

      // only moves up 2 squares if in the starting position
      if(pos_to_u64(ix) & START_BP) {
        dst = src >> 16ULL;

        // pawn only moves forward in non-attack
        if(!piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      }

      // check attack positions
      if(files[0] & src) {
        dst = src >> 9ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      } else if(files[BOARDLEN - 1] & src) {
        dst = src >> 7ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      } else {
        dst = src >> 7ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
        dst = src >> 9ULL;
        if(piece_type_at_u64(&board, dst)) {
          _add_move_if_valid(src, dst, 0, type);
        }
      }
    }
  }
}

/**
 * Generate moves for a knight
 *
 * @param type The type of piece to generate moves for
 */
static void _gen_moves_knight(enum piece type) {
  u64 bitboard, src, dst;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {
      src = pos_to_u64(ix);
      // move accordingly
      board_loop(kx) {
        if(knight_moves[ix] & pos_to_u64(kx)) {
          dst = pos_to_u64(kx);
          _add_move_if_valid(src, dst, 0, type);
        }
      }
    }
  }
}

/**
 * Generate moves for a bishop
 *
 * @param type The type of piece to generate moves for
 */
static void _gen_moves_bishop(enum piece type) {
  u64 bitboard, src;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {
      src = pos_to_u64(ix);
      // move along diagonals
      _moves_diagonal(src, bitboard, type);
    }
  }
}

/**
 * Generate moves for a rook
 *
 * @param type The type of piece to generate moves for
 */
static void _gen_moves_rook(enum piece type) {
  u64 bitboard, src;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {
      src = pos_to_u64(ix);
      // move along ranks + files
      _moves_rank_file(src, bitboard, type);
    }
  }
}

/**
 * Generate moves for a queen
 *
 * @param type The type of piece to generate moves for
 */
static void _gen_moves_queen(enum piece type) {
  u64 bitboard, src;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {
      src = pos_to_u64(ix);
      // move along diagonals
      _moves_diagonal(src, bitboard, type);
      _moves_rank_file(src, bitboard, type);
    }
  }
}

/**
 * Generate moves for a king
 *
 * @param type The type of piece to generate moves for
 */
static void _gen_moves_king(enum piece type) {
  u64 bitboard, src, dst;
  bitboard = board.bitboards[type];
  board_loop(ix) {
    if(has_piece_at(bitboard, ix)) {
      src = pos_to_u64(ix);

      // move accordingly
      board_loop(kx) {
        if(king_moves[ix] & pos_to_u64(kx)) {
          dst = pos_to_u64(kx);
          _add_move_if_valid(src, dst, 0, type);
        }
      }

      // check castles
      if((type == WK) &&
        (board.WK_castle_king) &&
        (pos_to_u64(ix) & START_WK) &&
        (board.bitboards[WR] & WR_KING)) {
          dst = WK_CASTLE_KING;
          _add_move_if_valid(src, dst, RANK_8, type);
      } else if((type == WK) &&
        (board.WK_castle_queen) &&
        (pos_to_u64(ix) & START_WK) &&
        (board.bitboards[WR] & WR_QUEEN)) {
          dst = WK_CASTLE_QUEEN;
          _add_move_if_valid(src, dst, RANK_8, type);
      } else if((type == BK) &&
        (board.BK_castle_king) &&
        (pos_to_u64(ix) & START_BK) &&
        (board.bitboards[BR] & BR_KING)) {
          dst = BK_CASTLE_KING;
          _add_move_if_valid(src, dst, RANK_1, type);
      } else if((type == BK) &&
        (board.BK_castle_queen) &&
        (pos_to_u64(ix) & START_BK) &&
        (board.bitboards[BR] & BR_QUEEN)) {
          dst = BK_CASTLE_QUEEN;
          _add_move_if_valid(src, dst, RANK_1, type);
      }

    // only ever 1 king
    break;
    }
  }
}

/**
 * Generate moves for a pices based on type
 *
 * @param type The type of piece to generate moves for
 */
void gen_moves_piece(enum piece type) {
  switch(type) {
    case WP:
      _gen_moves_pawn_white();
      break;
    case BP:
      _gen_moves_pawn_black();
      break;
    case WN:
    case BN:
      _gen_moves_knight(type);
      break;
    case WB:
    case BB:
      _gen_moves_bishop(type);
      break;
    case WR:
    case BR:
      _gen_moves_rook(type);
      break;
    case WK:
    case BK:
      _gen_moves_king(type);
      break;
    case WQ:
    case BQ:
      _gen_moves_queen(type);
      break;
    case NONE:
    default:
      break;
  }
}
