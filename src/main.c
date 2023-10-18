#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/keycodes.h>
#include <gint/clock.h>

#define CELL_SIZE 4
#define GRID_WIDTH (128 / CELL_SIZE)
#define GRID_HEIGHT (64 / CELL_SIZE)
#define GRID_LENGTH (GRID_WIDTH * GRID_HEIGHT)

struct coord
{
	unsigned char x;
	unsigned char y;
};

unsigned char grid[GRID_LENGTH];
struct coord coords[GRID_LENGTH];
int coords_index = 1;

void draw_VRAM()
{
	int x;
	int y;
	int i;
	dclear(C_WHITE);
	for (i = 0; i < GRID_LENGTH; i++)
	{
		x = (i % GRID_WIDTH) * CELL_SIZE;
		y = (i / GRID_WIDTH) * CELL_SIZE;
		if (grid[i])
		{
			drect(x, y, x + CELL_SIZE - 1, y + CELL_SIZE - 1, C_BLACK);
		}
	}
}

int main(void)
{
	unsigned char x = 0;
	unsigned char y = 0;

	/*
	0 - >
	1 - v
	2 - <
	3 - ^
	*/
	unsigned char dir = 0;

	int i; // for loop variable

	dclear(C_WHITE);

	while (1)
	{
		if (keydown(KEY_RIGHT))
		{
			dir = 0;
		}
		if (keydown(KEY_DOWN))
		{
			dir = 1;
		}
		if (keydown(KEY_LEFT))
		{
			dir = 2;
		}
		if (keydown(KEY_UP))
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

		if (keydown(KEY_EXIT))
		{
			break;
		}

		x = (x) % GRID_WIDTH;
		y = (y) % GRID_HEIGHT;

		grid[x + y * GRID_WIDTH] = 1;
		grid[coords[0].x + coords[0].y * GRID_WIDTH] = 0;

		for (i = 0; i < coords_index; i++)
		{
			coords[i] = coords[i + 1];
		}
		coords[coords_index].x = x;
		coords[coords_index].y = y;

		draw_VRAM();
		dupdate();
		sleep_ms(50);
	}
	return 1;
}