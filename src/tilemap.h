#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

//We need to store only the {alive} parameter
static bool grid[COLUMNS][ROWS];

void tilemap_initialise();
void tilemap_randomize();

bool get_tile(int column, int row);
void set_tile(int column, int row, bool alive);
unsigned short get_neighbors(int column, int row, bool tilemap[COLUMNS][ROWS]);

#endif
