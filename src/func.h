#include "dijkstra.h"
#include <glib.h>

#define pos_to_u64(pos) (1ULL << pos)

u64 coord_to_u64(char file, char rank);
void u64_to_coord(u64 pos, char *str, int size);
enum color color(enum piece piece);
int high_bit(u64 bitboard);
int low_bit(u64 bitboard);
