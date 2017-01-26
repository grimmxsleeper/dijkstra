#include <glib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "board.h"
#include "bitboards.h"
#include "masks.h"
#include "movegen.h"

static bool _exiting = false;

const char *piece_strings[PIECE_TYPES] = {
  "  ", "WP", "WN", "WB", "WR", "WQ", "WK", "BP", "BN", "BB", "BR", "BQ", "BK"
};
enum piece white_pieces[PIECE_PER_COLOR] = {
  WP, WN, WB, WR, WQ, WK
};
enum piece black_pieces[PIECE_PER_COLOR] = {
  BP, BN, BB, BR, BQ, BK
};
const u64 ranks[BOARDLEN] = {
  RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};
const u64 files[BOARDLEN] = {
  FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};
const u64 diags_r[NUM_DIAG] = {
  DL_1, DL_2, DL_3, DL_4, DL_5, DL_6, DL_7, DL_8,
  DL_9, DL_10, DL_11, DL_12, DL_13, DL_14, DL_15
};
const u64 diags_l[NUM_DIAG] = {
  DR_1, DR_2, DR_3, DR_4, DR_5, DR_6, DR_7, DR_8,
  DR_9, DR_10, DR_11, DR_12, DR_13, DR_14, DR_15
};
const u64 knight_moves[BOARDSIZE] = {
  NM_0, NM_1, NM_2, NM_3, NM_4, NM_5, NM_6, NM_7, NM_8, NM_9,
  NM_10, NM_11, NM_12, NM_13, NM_14, NM_15, NM_16, NM_17, NM_18, NM_19,
  NM_20, NM_21, NM_22, NM_23, NM_24, NM_25, NM_26, NM_27, NM_28, NM_29,
  NM_30, NM_31, NM_32, NM_33, NM_34, NM_35, NM_36, NM_37, NM_38, NM_39,
  NM_40, NM_41, NM_42, NM_43, NM_44, NM_45, NM_46, NM_47, NM_48, NM_49,
  NM_50, NM_51, NM_52, NM_53, NM_54, NM_55, NM_56, NM_57, NM_58, NM_59,
  NM_60, NM_61, NM_62, NM_63
};

/**
 * Check command line arguments
 *
 * @param args Argument array
 * @param req_args Number of required arguments
 * @return If there are at least req_args then return true otherwise false.
 */
static bool _cmd_check_args(gchar **args, int req_args)
{
  if(!req_args) {
    return true;
  }
  if(args &&
     g_strv_length(args) >= req_args) {
    return true;
  }
  printf("Error: Command %s expected %d arguments but only %d were passed in\n",
    args[0], req_args, g_strv_length(args));
  return false;
}

/**
 * Perform command uci
 *
 * @param args Argument array
 */
static void _cmd_uci(gchar **args)
{
  printf("id name %s\n", NAME);
  printf("id author %s\n", AUTHOR);
  printf("uciok\n");
}

/**
 * Check if we are ready
 *
 * @param args Argument array
 */
static void _cmd_is_ready(gchar **args)
{
  printf("readyok\n");
}

/**
 * Start a new game
 *
 * @param args Argument array
 */
static void _cmd_new_game(gchar **args)
{
  printf("Game accepted\n");
}

/**
 * Setup a board and set starting positions
 *
 * @param args Argument array
 */
static void _cmd_position(gchar **args)
{
  if(!_cmd_check_args(args, 2)) {
    return;
  }
  if(g_strv_length(args) > 2) {
    if(!streq(args[2], "moves")) {
      printf("Error: Did not find moves\n");
      printf("Expected: position startpos moves [MOVE] [MOVE] .. ]\n");
      return;
    }
  }
  board_clear();
  // Based an arguments perform initial set of moves
  for(int ix = 3; ix < g_strv_length(args); ix++) {
    board_make_move(args[ix]);
  };
  board_print_turn();
  generate_moves();
  board_print();
}

/**
 * Set that we are exiting
 *
 * @param args Argument array
 */
static void _cmd_exit(gchar **args)
{
  _exiting = true;
}

/**
 * Print the bitboards for the pieces
 *
 * @param args Argument array
 */
static void _cmd_printbb(gchar **args)
{
  board_print_bitboard();
}

/**
 * Print the current board
 *
 * @param args Argument array
 */
static void _cmd_print(gchar **args)
{
  board_print();
}

/**
 * Perform a move based on the arguments
 *
 * @param args Argument array
 */
static void _cmd_testmove(gchar **args)
{
  if(!_cmd_check_args(args, 2)) {
    return;
  }
  board_make_move(args[1]);
  board_print();
}

/**
 * Perform a test
 *
 * @param args Argument array
 */
static void _cmd_test(gchar **args)
{
  u64 test_bb;
  u64 rank_bb = 0, file_bb = 0, dr_bb = 0, dl_bb = 0;
  test_bb = START_WQ << 8ULL;
  print_pretty_string_newline(WQ);
  print_single_bitboard(test_bb, WQ);
  boardlen_loop(ix) {
    if(ranks[ix] & test_bb) {
      rank_bb = ranks[ix];
    }
    if(files[ix] & test_bb) {
      file_bb = files[ix];
    }
  }
  diag_num_loop(ix) {
    if(diags_r[ix] & test_bb) {
      dr_bb = diags_r[ix];
    }
    if(diags_l[ix] & test_bb) {
      dl_bb = diags_l[ix];
    }
  }
  print_single_bitboard(
    rank_bb | file_bb | dr_bb | dl_bb,
    WQ
  );
}

static struct board_command commands[] =
{
  {"help", print_help},
  {"?", print_help},
  {"uci", _cmd_uci},
  {"exit", _cmd_exit},
  {"bye", _cmd_exit},
  {"isready", _cmd_is_ready},
  {"ucinewgame", _cmd_new_game},
  {"position", _cmd_position},
  {"printbb", _cmd_printbb},
  {"print", _cmd_print},
  //{"go", _cmd_go},
  {"stop", _cmd_exit},
  {"quit", _cmd_exit},
  {"testmove", _cmd_testmove},
  {"test", _cmd_test},
};

/**
 * Show the available commands
 */
void print_help(gchar **args)
{
  printf("Available commands\n");
  printf("==================\n");
  int ix = 0;
  int num = sizeof(commands) / sizeof(struct board_command);
  for(ix = 0; ix < num; ix++) {
    printf("%s\n", commands[ix].cmd);
  }
  printf("\n");
}

/**
 * The main execution of the program
 */
int main(void) {
  int ix;
  int max_tokens = 100;
  int num_commands = sizeof(commands) / sizeof(struct board_command);
  gchar **args = NULL;
  char *action = NULL;

  char line[2048];
  bool valid_action;
  board_init();

  printf("Welcome to Dijkstra!\n");
  printf("Type 'help' for information\n");
  while(1) {
    if(_exiting) {
      printf("Thanks for playing. Bye for now\n");
      return 0;
    }
    printf(">> ");
    // take input and output from uci standard
    if(!fgets(line, sizeof(line), stdin)) {
      print_help(NULL);
      _exiting = true;
      continue;
    }
    g_strstrip(line);
    line[strcspn(line, "\n")] = 0;
    args = g_strsplit(line, " ", max_tokens);
    if(!args) {
      printf("Error: Failed to parse line: %s\n", line);
      continue;
    }
    if(!g_strv_length(args)) {
      g_strfreev(args);
      continue;
    }
    action = args[0];
    valid_action = false;
    for(ix = 0; ix < num_commands; ix++) {
      if(streq(commands[ix].cmd, action)) {
        commands[ix].func(args);
        valid_action = true;
        break;
      }
    }
    if(!valid_action) {
      printf("Error: %s is not a valid action!\n", action);
      printf("Type 'help' for valid actions.\n");
    }
    g_strfreev(args);
    args = NULL;
  }
}
