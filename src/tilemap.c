#include "tilemap.h"

int max(int a, int b){
  return (a > b) ? a : b;
}
int min(int a, int b){
  return (a < b) ? a : b;
}

void tilemap_initialise() {
	srand(time(NULL));

	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			grid[x][y] = false;
		}
	}
}

void tilemap_randomize() {
	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			grid[x][y] = (1 + rand() % 101 <= 50);
		}
	}
}


bool get_tile(int column, int row) {
	return grid[column][row];
}

void set_tile(int column, int row, bool alive) {
	grid[column][row] = alive;
}

unsigned short get_neighbors(int column, int row, bool tilemap[COLUMNS][ROWS]) {
	unsigned short count = 0;
	for (int x = max(0, row - 1); x <= min(row + 1, ROWS-1); x++) {
		for (int y = max(0, column - 1); y <= min(column + 1, COLUMNS-1); y++) {
			if ((y != column || x != row) && tilemap[y][x])
				count++;
		}
	}
	return count;
}
