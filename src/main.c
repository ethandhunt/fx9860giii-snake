#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/keycodes.h>
#include <gint/clock.h>
#include <stdlib.h>

#define CELL_SIZE 4
#define GRID_WIDTH (128 / CELL_SIZE)
#define GRID_HEIGHT (64 / CELL_SIZE)
#define GRID_LENGTH (GRID_WIDTH * GRID_HEIGHT)

#define DELAY_LENGTH 200

#define COORDS_INDEX_START 0

struct coord
{
	unsigned char x;
	unsigned char y;
};

unsigned char grid[GRID_LENGTH];
struct coord coords[GRID_LENGTH];
int coords_index = COORDS_INDEX_START;

struct coord apple_coord;

void draw()
{
	int x;
	int y;
	dclear(C_WHITE);
	for (int i = 0; i < GRID_LENGTH; i++)
	{
		x = (i % GRID_WIDTH) * CELL_SIZE;
		y = (i / GRID_WIDTH) * CELL_SIZE;
		if (grid[i])
		{
			drect(x, y, x + CELL_SIZE - 1, y + CELL_SIZE - 1, C_BLACK);
		}
	}

	x = apple_coord.x * CELL_SIZE;
	y = apple_coord.y * CELL_SIZE;
	drect_border(x, y, x + CELL_SIZE-1, y + CELL_SIZE-1, C_WHITE, 1, C_BLACK);
}

int main(void)
{
	unsigned char x = GRID_HEIGHT/2;
	unsigned char y = GRID_WIDTH/2;

	/*
	0 - >
	1 - v
	2 - <
	3 - ^
	*/
	unsigned char dir = 0;

	dclear(C_WHITE);

	while (1)
	{
		clearevents();
		if (keydown(KEY_RIGHT) && dir != 2)
		{
			dir = 0;
		}
		else if (keydown(KEY_DOWN) && dir != 3)
		{
			dir = 1;
		}
		else if (keydown(KEY_LEFT) && dir != 0)
		{
			dir = 2;
		}
		else if (keydown(KEY_UP) && dir != 1)
		{
			dir = 3;
		}

		if (dir == 0)
		{
			x++;
		}
		else if (dir == 1)
		{
			y++;
		}
		else if (dir == 2)
		{
			x--;
		}
		else if (dir == 3)
		{
			y--;
		}

		if (x == apple_coord.x && y == apple_coord.y)
		{
			coords_index++;
			apple_coord.x = rand() % GRID_WIDTH;
			apple_coord.y = rand() % GRID_HEIGHT;
		}

		if (keydown(KEY_EXIT))
		{
			break;
		}

		// x < 0 || y < 0 unnecessary because unsigned int wraps around
		if (grid[x + y*GRID_WIDTH] ||x >= GRID_WIDTH || y >= GRID_HEIGHT)
		{
			dprint(1, 1, C_BLACK, "LOSE");
			dupdate();
			getkey();

			x = GRID_WIDTH/2;
			y = GRID_HEIGHT/2;
			coords_index = COORDS_INDEX_START;
			dir = 0;
			for (int i=0; i<GRID_LENGTH; i++)
			{
				grid[i] = 0;
			}
		}

		grid[x + y * GRID_WIDTH] = 1;
		grid[coords[0].x + coords[0].y * GRID_WIDTH] = 0;

		for (int i = 0; i < coords_index; i++)
		{
			coords[i] = coords[i + 1];
		}
		coords[coords_index].x = x;
		coords[coords_index].y = y;

		draw();
		dupdate();
		sleep_ms(DELAY_LENGTH);
	}
	return 1;
}