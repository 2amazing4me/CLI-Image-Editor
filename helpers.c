// Copyright 2023 Baldovin Razvan-Mihai-Marian 312CA

#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"

// Removes newline characters before the string terminator
void trim(char **str)
{
	int len = strlen(*str);
	if ((*str)[len - 1] == '\n')
		(*str)[len - 1] = 0;
}

// Reads command parameters
void read_param(char **param)
{
	/*
	strtok stores a global NULL variable, therefore we can use NULL without the
	initial string and continue reading tokens from the same string
	*/
	char *p = strtok(NULL, " \0");
	if (p) {
		strcpy(*param, p);
		trim(param);
	}
}

int check_image_loaded(int img_loaded)
{
	if (!img_loaded)
		printf("No image loaded\n");

	return img_loaded;
}

// Checks if the coordinates are valid for selection
int check_coords(int x1, int y1, int x2, int y2, int lin, int col)
{
	if (x1 == x2 || y1 == y2)
		return 0;
	if (x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0)
		return 0;
	if (x1 > col || y1 > lin || x2 > col || y2 > lin)
		return 0;
	return 1;
}

// Checks if there is a parameter, and if so, checks if it's a number
int check_param(int val, char *param, char *str, int *pos)
{
	if ((val == 0 && param[0] != '0') || !str[*pos]) {
		printf("Invalid command\n");
		free(param);
		return 0;
	}

	*pos += strlen(param) + 1;

	return 1;
}

double min(double a, double b)
{
	if (a < b)
		return a;
	return b;
}

double max(double a, double b)
{
	if (a > b)
		return a;
	return b;
}

void clamp(pixel ***arr, int lin, int col, int max_val_pixel)
{
	for (int i = 0; i < lin; ++i)
		for (int j = 0; j < col; ++j) {
			(*arr)[i][j].r = min(max((*arr)[i][j].r, 0), max_val_pixel);
			(*arr)[i][j].g = min(max((*arr)[i][j].g, 0), max_val_pixel);
			(*arr)[i][j].b = min(max((*arr)[i][j].b, 0), max_val_pixel);
			(*arr)[i][j].value = min(max((*arr)[i][j].value, 0), max_val_pixel);
		}
}

int in_selection(int i, int j, area *active)
{
	if ((i >= (*active).y1 && i < (*active).y2) &&
		(j >= (*active).x1 && j < (*active).x2))
		return 1;
	return 0;
}

int on_border(int i, int j, int lin, int col)
{
	if (i == 0 || j == 0 || i == lin - 1 || j == col - 1)
		return 1;
	return 0;
}

void free_arr(pixel ***arr, int lin)
{
	for (int i = 0; i < lin; ++i)
		free((*arr)[i]);
	free(*arr);
}

#endif // HELPERS_H
