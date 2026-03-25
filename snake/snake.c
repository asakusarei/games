#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define X 20
#define Y 10
#define FOOD_SPAWN_DELAY 20

int score;
int time_elapsed;
bool food_status = false;

void clear_screen()
{
	printf("\n\033[H\033[J");
}

void spawn_food(int *food_x, int *food_y)
{	
	do
	{
		*food_x = rand() % 10;
		*food_y = rand() % 10;
	}
	while (*food_x > X-2 || *food_x == 0 || *food_y > Y-2 || *food_y == 0);
}

void draw_field(int x, int y, int *food_x, int *food_y)
{
	for (int i = 0; i < Y; i++)
	{
		for (int j = 0; j < X; j++)
		{	
			if (j == x && i == y)
			{
				printf("o");
				if (j == *food_x && i == *food_y)
				{
					score++;
					food_status = false;
				}
			}
			else if (j == *food_x && i == *food_y && food_status == true)
			{
					printf("*");
			}
			else if (i == 0 || i == Y - 1 || j == 0 || j == X - 1)
			{
				printf("#");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void stat_screen(struct timespec start, struct timespec current)
{
	double time_elapsed = 
		(current.tv_sec - start.tv_sec) + 
		(current.tv_nsec - start.tv_nsec) / 1e9;
	
	printf("\ntime elapsed: %.2f\n", time_elapsed);
	printf("score: %d\n", score);
	
}

int main(void)
{
	srand(time(NULL));
	
	int x = X/2;
	int y = Y/2;
	int dx = 0;
	int dy = 0;
	
	int food_x;
	int food_y;
	int iter_count = 0;
	
	struct timespec start, current;
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	while (1)
	{
		if (_kbhit())
		{
			char ch = _getch();

			switch (ch)
			{
				case 'w':
				{
					dx = 0;
					dy = -1;
					break;
				}
				case 'a':
				{
					dx = -1;
					dy = 0;
					break;
				}
				case 'd':
				{
					dx = 1;
					dy = 0;
					break;
				}
				case 's':
				{
					dx = 0;
					dy = 1;
					break;
				}
			}
		}
		
		x += dx;
		y += dy;
		
		if (x < 1)
		{
			x = 1;
		}
		if (x >= X-2)
		{
			x = X-2;
		}
		if (y < 1)
		{
			y = 1;
		}
		if (y >= Y-2)
		{
			y = Y-2;
		}

		clear_screen();
		
		if (iter_count > FOOD_SPAWN_DELAY)
		{
			spawn_food(&food_x, &food_y);
			food_status = true;
			iter_count = 0;
		}	
		
		iter_count++;
		
		draw_field(x, y, &food_x, &food_y);
		
		clock_gettime(CLOCK_MONOTONIC, &current);
		stat_screen(start, current);
	
		Sleep(300);
	}
	
	return 0;
}