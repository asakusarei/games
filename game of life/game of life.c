#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include "../util/double_buffer.h"
#include "../util/platform.h"

#define WIDTH 100
#define HEIGHT 40

// void render(char* buffer, char* screen)
// {
	// strcpy(buffer, screen);
// }

void display(char* buffer)
{
	printf("\n\033[H");
	printf("%s\n", buffer);
	fflush(stdout);
}

char** make_field()
{
	char** field = malloc(sizeof(char*) * HEIGHT);
	
	for (int i = 0; i < HEIGHT; i++)
	{
		field[i] = malloc(sizeof(char) * WIDTH);
		
		for (int j = 0; j < WIDTH; j++)
		{
			field[i][j] = ' ';
		}
	}
	
	return field;
}

char** fill_field(char** field)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (rand() % 2)
			{
				field[i][j] = '*';
			}
			else 
			{
				field[i][j] = ' ';
			}
		}
	}
	
	return field;
}

void free_memory(char** field)
{
	for (int i = 0; i < HEIGHT; i++)
	{
		free(field[i]);
	}
	
	free(field);
}

char** update_generation(char** current_gen)
{
	char** new_gen = malloc(sizeof(char*) * HEIGHT);
	int neighbor_count = 0;
	
	for (int i = 0; i < HEIGHT; i++)
	{
		new_gen[i] = malloc(sizeof(char) * WIDTH);
		
		for (int j = 0; j < WIDTH; j++)
		{
			if (j + 1 < WIDTH && current_gen[i][j+1] == '*') {neighbor_count++;}
			if (j - 1 >= 0 && current_gen[i][j-1] == '*') {neighbor_count++;}
			if (i + 1 < HEIGHT && current_gen[i+1][j] == '*') {neighbor_count++;}
			if (i - 1 >= 0 && current_gen[i-1][j] == '*') {neighbor_count++;}
			if (i + 1 < HEIGHT && j + 1 < WIDTH && current_gen[i+1][j+1] == '*') {neighbor_count++;}
			if (i - 1 >= 0 && j + 1 < WIDTH && current_gen[i-1][j+1] == '*') {neighbor_count++;}
			if (i + 1 < HEIGHT && j - 1 >= 0 && current_gen[i+1][j-1] == '*') {neighbor_count++;}
			if (i - 1 >= 0 && j - 1 >= 0 && current_gen[i-1][j-1] == '*') {neighbor_count++;}
			
			if (current_gen[i][j] == ' ' && neighbor_count == 3) {new_gen[i][j] = '*';}
			else if (current_gen[i][j] == '*' && neighbor_count <= 3 && neighbor_count >= 2) {new_gen[i][j] = '*';}
			else {new_gen[i][j] = ' ';}
			neighbor_count = 0;
		}
	}
	free_memory(current_gen);
	
	return new_gen;
}

void print_field(char** current_gen, struct timespec start, struct timespec current, char* buffer)
{
	char screen[HEIGHT * (WIDTH + 3) + 2000];
	char *p = buffer;
	
	
	for (int i = 0; i < WIDTH + 2; i++) 
	{
		*p++ = '#';
	}

	*p++ = '\n';
	
	for (int i = 0; i < HEIGHT; i++)
	{
		*p++ = '#';
		
		for (int j = 0; j < WIDTH; j++)
		{
			*p++ = current_gen[i][j];
		}
		
		*p++ = '#';
		*p++ = '\n';
	}
	
	for (int i = 0; i < WIDTH + 2; i++) 
	{
		*p++ = '#';
	}
	*p++ = '\n';
	
	int unit_counter = 0;
	
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			if (current_gen[i][j] == '*') 
				unit_counter++;
	
	double elapsed = 
		(current.tv_sec - start.tv_sec) + 
		(current.tv_nsec - start.tv_nsec) / 1e9;
	
	p += sprintf(p, "\nalive units: %d\n", unit_counter);
	p += sprintf(p, "\ntime ellapsed: %.2f\n", elapsed);
	p += sprintf(p, "\nX: %d; Y: %d", WIDTH, HEIGHT);
	
	*p = '\0';
	// render(buffer, screen);
	unit_counter = 0;
}

int main(void)
{
	srand(time(NULL));
	char** current_gen = make_field();
	current_gen = fill_field(current_gen);
	bool status = true;
	struct timespec start, current;
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	char buffer1[HEIGHT * (WIDTH + 3) + 2000];
	char buffer2[HEIGHT * (WIDTH + 3) + 2000];
	
	char* front = buffer1;
	char* back = buffer2;
	
	do
	{
		if (status)
		{
			clock_gettime(CLOCK_MONOTONIC, &current);
			print_field(current_gen, start, current, back);
			display(back);
			sleep_ms(1000);
			swap_buffers(&front, &back);
			current_gen = update_generation(current_gen);
		}
		
		if (_kbhit())
		{
			int ch = _getch();
			
			if (ch == 'p')
			{
				if (status)
				{
					printf("\n[PAUSED]\n");
					status = false;
				}
				else {status = true;}
			}
			else if (ch == 'x')
			{
				printf("\nsainara~\n");
				free_memory(current_gen);
				
				return 0;
			}
		
		}
		
	}
	while (1);
	
	return 0;
}
