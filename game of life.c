#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 10

char** make_field()
{
	char** field = malloc(sizeof(char*) * WIDTH);
	
	for (int i = 0; i < WIDTH; i++)
	{
		field[i] = malloc(sizeof(char) * HEIGHT);
		
		for (int j = 0; j < HEIGHT; j++)
		{
			field[i][j] = ' ';
		}
	}
	
	return field;
}

char** fill_field(char** field)
{
	srand(time(NULL));
	
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			int r = rand() % WIDTH;
			
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
	for (int i = 0; i < WIDTH; i++)
	{
		free(field[i]);
	}
	free(field);
}

char** update_generation(char** current_gen)
{
	char** new_gen = malloc(sizeof(char*) * WIDTH);
	int neighbor_count = 0;
	
	for (int i = 0; i < WIDTH; i++)
	{
		new_gen[i] = malloc(sizeof(char) * HEIGHT);
		for (int j = 0; j < HEIGHT; j++)
		{
			if (j + 1 < HEIGHT && current_gen[i][j+1] == '*') {neighbor_count++;}
			if (j - 1 >= 0 && current_gen[i][j-1] == '*') {neighbor_count++;}
			if (i + 1 < WIDTH && current_gen[i+1][j] == '*') {neighbor_count++;}
			if (i - 1 >= 0 && current_gen[i-1][j] == '*') {neighbor_count++;}
			if (i + 1 < WIDTH && j + 1 < HEIGHT && current_gen[i+1][j+1] == '*') {neighbor_count++;}
			if (i - 1 >= 0 && j + 1 < HEIGHT && current_gen[i-1][j+1] == '*') {neighbor_count++;}
			if (i + 1 < WIDTH && j - 1 >= 0 && current_gen[i+1][j-1] == '*') {neighbor_count++;}
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

void print_field(char** current_gen)
{
	for (int i = 0; i < WIDTH; i++)
	{
		printf("[");
		for (int j = 0; j < HEIGHT; j++)
		{
			if (j == 0)
			{
				printf("|%c|", current_gen[i][j]);
			}
			else
			{
				printf("%c|", current_gen[i][j]);
			}
		}
		printf("]\n");
	}
}



int main(void)
{
	char** current_gen = make_field();
	current_gen = fill_field(current_gen);
	
	do
	{
		system("cls");
		print_field(current_gen);
		printf("\n");
		Sleep(1000);

		current_gen = update_generation(current_gen);
	}
	while (1);
	
	return 0;
}