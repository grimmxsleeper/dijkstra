#include <glib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "board.h"
#include "bitboards.h"
#include "masks.h"

const char *piece_strings[PIECE_TYPES] = {
  "  ", "WP", "WN", "WB", "WR", "WQ", "WK", "BP", "BN", "BB", "BR", "BQ", "BK"
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

int main(void) {
  char line[1024];
  GList *cmd = NULL;
  char *token;
  int cmd_len = 0;
  //enum colors color;
  struct board *board = NULL;
  while(1) {
    // take input and output from uci standard
    if(fgets(line, sizeof(line), stdin)) {
      line[strcspn(line, "\n")] = 0;
      cmd = NULL;
      token = strtok(line, " ");
      while(token) {
        if(strlen(token)) {
          cmd = g_list_prepend(cmd, strdup(token));
        }
        token = strtok(NULL, " ");
      }
      if(cmd) {
        cmd = g_list_reverse(cmd);
        cmd_len = g_list_length(cmd);
        if(cmd_len) {
          if(streq(cmd->data, "uci")) {
            printf("id name %s\n", NAME);
            printf("id author %s\n", AUTHOR);
            printf("uciok\n");
          } else if(streq(cmd->data, "isready")) {
            printf("readyok\n");
          } else if(streq(cmd->data, "ucinewgame")) {
            printf("game accepted lolololol\n");;
          } else if(streq(cmd->data, "position")) {
            if(board) {
              free(board);
            }
            board = calloc(1, sizeof(struct board));
            if(cmd->next && streq(cmd->next->data, "startpos")) {
              setup_board(board, cmd->next->next);
              print_board(board);
            } else {
              printf(BAD_CMD_STR);
            }
          } else if(streq(cmd->data, "printbb")) {
            if(board) {
              piece_loop(ix) {
                print_single_bitboard(board->bitboards[ix], ix);
              }
            }
          } else if(streq(cmd->data, "print")) {
            print_board(board);
          } else if(streq(cmd->data, "go")) {
            ;
          } else if(streq(cmd->data, "stop")) {
            ;
          } else if(streq(cmd->data, "quit")) {
            if(board) {
              free(board);
            }
            break;
          } else if(streq(cmd->data, "testmove")) {
            if(cmd->next) {
              if(board) {
                make_move_from_uci_str(board, cmd->next->data);
                print_board(board);
              }
            }
          } else if(streq(cmd->data, "test")) {
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
          } else {
            printf(BAD_CMD_STR);
          }
          g_list_free_full(cmd, g_free);
        }
      }
    }
  }
}
