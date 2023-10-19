#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/keycodes.h>
#include <gint/clock.h>
#include <stdlib.h>
#include <gint/timer.h>

#define CELL_SIZE 4
#define GRID_WIDTH (128 / CELL_SIZE)
#define GRID_HEIGHT (64 / CELL_SIZE)
#define GRID_LENGTH (GRID_WIDTH * GRID_HEIGHT)

#define KEY_POLL_DELAY 1000  // μs
#define DELAY_LENGTH 100     // ms

#define COORDS_INDEX_START 0

struct coord
{
	unsigned char x;
	unsigned char y;
};

enum DIR {
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
	DIR_UP
};

unsigned char grid[GRID_LENGTH];
struct coord coords[GRID_LENGTH];
int coords_index = COORDS_INDEX_START;

struct coord apple_coord;

enum DIR dir = DIR_RIGHT;
enum DIR next_dir = DIR_RIGHT;

int key_poll_count = 0;

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
	dprint(50, 0, C_BLACK, "%d", key_poll_count);
}

int input_handler()
{
	key_poll_count++;
	clearevents();
	if (keydown(KEY_RIGHT) && dir != DIR_LEFT)
	{
		next_dir = DIR_RIGHT;
	}
	else if (keydown(KEY_DOWN) && dir != DIR_UP)
	{
		next_dir = DIR_DOWN;
	}
	else if (keydown(KEY_LEFT) && dir != DIR_RIGHT)
	{
		next_dir = DIR_LEFT;
	}
	else if (keydown(KEY_UP) && dir != DIR_DOWN)
	{
		next_dir = DIR_UP;
	}

	return TIMER_CONTINUE;
}

int main(void)
{
	unsigned char x = GRID_WIDTH/2;
	unsigned char y = GRID_HEIGHT/2;

	apple_coord.x = rand() % GRID_WIDTH;
	apple_coord.y = rand() % GRID_HEIGHT;

	int timer_id = timer_configure(TIMER_ANY, KEY_POLL_DELAY, GINT_CALL(input_handler));
	timer_start(timer_id);

	dclear(C_WHITE);

	while (1)
	{
		dir = next_dir;
		if (dir == DIR_RIGHT)
		{
			x++;
		}
		else if (dir == DIR_DOWN)
		{
			y++;
		}
		else if (dir == DIR_LEFT)
		{
			x--;
		}
		else if (dir == DIR_UP)
		{
			y--;
		}

		if (x == apple_coord.x && y == apple_coord.y)
		{
			coords_index++;
			apple_coord.x = rand() % GRID_WIDTH;
			apple_coord.y = rand() % GRID_HEIGHT;
		}

		clearevents();
		if (keydown(KEY_EXIT))
		{
			break;
		}

		// x < 0 || y < 0 unnecessary because unsigned int wraps around
		if (grid[x + y*GRID_WIDTH] ||x >= GRID_WIDTH || y >= GRID_HEIGHT)
		{
			dprint(1, 1, C_BLACK, "LOSE");
			dprint(1, 9, C_BLACK, "SCORE: %d", coords_index);
			dupdate();
			sleep_ms(DELAY_LENGTH);
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