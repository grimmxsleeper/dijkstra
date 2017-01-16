#include <glib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "board.h"

const char *piece_strings[PIECE_TYPES] = {
  "XX", "WP", "WN", "WB", "WR", "WQ", "WK", "BP", "BN", "BB", "BR", "BQ", "BK"
};

int main(void) {
  char line[1024];
  GList *cmd = NULL;
  GList *g_iter;
  char *token;
  int cmd_len = 0;
  enum colors color;
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
            board = calloc(1, sizeof(struct board));
            if(cmd->next && streq(cmd->next->data, "startpos")) {
              setup_board(board, cmd->next->next);
            } else {
              printf(BAD_CMD_STR);
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
          } else {
            printf(BAD_CMD_STR);
          }
          g_list_free_full(cmd, g_free);
        }
      }
    }
  }
}
