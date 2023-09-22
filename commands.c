// Copyright 2023 Baldovin Razvan-Mihai-Marian 312CA

#ifndef COMMANDS_H
#define COMMANDS_H

#define STR_SIZE 50

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"
#include "struct.h"

void load_file(pixel ***arr, area *active, int *img_type, int *lin, int *col,
			   int *max_val_pixel, int *img_loaded)
{
	char *file_name = (char *)calloc(STR_SIZE, sizeof(char));
	if (!file_name) {
		printf("Failed to calloc file_name (commands.c -> load_file)\n");
		return;
	}
	read_param(&file_name);

	FILE *image = fopen(file_name, "r");

	if (!image) {
		printf("Failed to load %s\n", file_name);
		free(file_name);
		return;
	}
	printf("Loaded %s\n", file_name);
	*img_loaded = 1;

	char magic_word[2];
	fscanf(image, "%s", magic_word);

	*img_type = magic_word[1] - '0';
	int is_binary = (*img_type > 3);
	*img_type = (*img_type - 1) % 3;

	// %*[#/a-zA-Z\n ] ignores all letters, spaces, newlines and the following
	// special characters(%, #, /)
	fscanf(image, "%*[%#/a-zA-Z\n ] %d %d %d %*[\n]", col, lin, max_val_pixel);

	*arr = (pixel **)malloc(*lin * sizeof(pixel *));
	if (!arr) {
		printf("Failed to malloc arr (commands.c -> load_file)\n");
		return;
	}
	for (int i = 0; i < *lin; ++i) {
		(*arr)[i] = (pixel *)calloc(*col, sizeof(pixel));
		if (!arr) {
			printf("Failes to calloc arr[i] (commands.c -> load_file)\n");
			return;
		}

		for (int j = 0; j < *col; ++j)
			if (*img_type == 2) {
				if (is_binary) {
					unsigned char red, green, blue;

					fread(&red, sizeof(char), 1, image);
					fread(&green, sizeof(char), 1, image);
					fread(&blue, sizeof(char), 1, image);

					(*arr)[i][j].r = red;
					(*arr)[i][j].g = green;
					(*arr)[i][j].b = blue;
				} else {
					fscanf(image, "%lf %lf %lf", &(*arr)[i][j].r,
						   &(*arr)[i][j].g, &(*arr)[i][j].b);
				}
			} else {
				if (is_binary) {
					unsigned char val;

					fread(&val, sizeof(char), 1, image);

					(*arr)[i][j].value = val;
				} else {
					fscanf(image, "%lf", &(*arr)[i][j].value);
				}
			}
	}

	// Sets the current working area to the whole image
	(*active).x1 = (*active).y1 = 0;
	(*active).x2 = *col;
	(*active).y2 = *lin;

	free(file_name);
	fclose(image);
}

void select_area(area *active, int lin, int col, char *str)
{
	char *param = (char *)calloc(STR_SIZE, sizeof(char));
	if (!param) {
		printf("Failed to calloc param (commands.c -> select_area)\n");
		return;
	}
	read_param(&param);

	// If we read the word 'ALL' we select the whole image, otherwise we read
	// the coordinates and select the area described by those coordinates
	if (strcmp(param, "ALL") == 0) {
		(*active).x1 = (*active).y1 = 0;
		(*active).x2 = col;
		(*active).y2 = lin;

		printf("Selected ALL\n");
	} else {
		// 'pos' stores the current position in the string after the command. It
		// is used for checking parameters of the command
		int pos = 7;

		int x1 = atol(param);
		if (!check_param(x1, param, str, &pos))
			return;
		read_param(&param);
		int y1 = atol(param);
		if (!check_param(y1, param, str, &pos))
			return;
		read_param(&param);
		int x2 = atol(param);
		if (!check_param(x2, param, str, &pos))
			return;
		read_param(&param);
		int y2 = atol(param);
		if (!check_param(y2, param, str, &pos))
			return;

		if (!check_coords(x1, y1, x2, y2, lin, col)) {
			printf("Invalid set of coordinates\n");
		} else {
			// We make sure the coordinates are ordered (top-left and
			// bottom-right corners)
			if (x2 < x1) {
				int aux = x1;
				x1 = x2;
				x2 = aux;
			}
			if (y2 < y1) {
				int aux = y1;
				y1 = y2;
				y2 = aux;
			}

			(*active).x1 = x1;
			(*active).y1 = y1;
			(*active).x2 = x2;
			(*active).y2 = y2;

			printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
		}
	}

	free(param);
}

void histogram(pixel **arr, area active, int img_type, int max_val_pixel,
			   char *str)
{
	char *param = (char *)calloc(STR_SIZE, sizeof(char));
	if (!param) {
		printf("Failed to calloc param (commands.c -> select_area)\n");
		free(param);
		return;
	}
	int stars, bins;

	// 'pos' stores the current position in the string after the command. It is
	// used for checking parameters of the command
	int pos = 10;
	read_param(&param);
	stars = atol(param);
	if (!check_param(stars, param, str, &pos))
		return;

	read_param(&param);
	bins = atol(param);
	if (!check_param(bins, param, str, &pos))
		return;

	free(param);

	if (str[pos]) {
		printf("Invalid command\n");
		return;
	}

	if (img_type == 2) {
		printf("Black and white image needed\n");
		return;
	}

	double bin_interval = max_val_pixel * 1.0 / bins;
	int *freq = (int *)calloc(bins, sizeof(int)), max_freq = 0;
	if (!freq) {
		printf("Failed to calloc freq (commands.c -> histogram)\n");
		return;
	}

	for (int i = active.y1; i < active.y2; ++i)
		for (int j = active.x1; j < active.x2; ++j) {
			// bin stores the bin the pixel belongs to
			int bin = arr[i][j].value / bin_interval;
			// if the pixel's value is the max value the bin calculation goes
			// one bin overboard, therefore we treat this case specially
			if (arr[i][j].value == max_val_pixel)
				--bin;
			++freq[bin];

			if (freq[bin] > max_freq)
				max_freq = freq[bin];
		}

	// We calculate the number of stars corresponding to each bin and print the
	// histogram accordingly
	for (int i = 0; i < bins; ++i) {
		double proportional_freq = freq[i] / (max_freq * 1.0);
		int star_num = proportional_freq * stars;

		printf("%d\t|\t", star_num);
		while (star_num) {
			printf("*");
			--star_num;
		}
		printf("\n");
	}

	free(freq);
}

void equalize(pixel ***arr, int lin, int col, int img_type, int max_val_pixel)
{
	if (img_type == 2) {
		printf("Black and white image needed\n");
		return;
	}

	pixel **new_arr = (pixel **)malloc(lin * sizeof(pixel *));
	if (!new_arr) {
		printf("Failed to malloc new_arr (commands.c -> equalize)\n");
		return;
	}
	for (int i = 0; i < lin; ++i) {
		new_arr[i] = (pixel *)calloc(col, sizeof(pixel));
		if (!new_arr) {
			printf("Failed to calloc new_arr's columns (commands.c -> "
				   "equalize)\n");
			return;
		}
	}

	int *freq = (int *)calloc(max_val_pixel + 1, sizeof(int));
	if (!freq) {
		printf("Failed to calloc freq (commands.c -> equalize\n");
		return;
	}
	for (int i = 0; i < lin; ++i)
		for (int j = 0; j < col; ++j)
			++freq[(int)(*arr)[i][j].value];

	/*
	The sum of all appearances of all values between 0 and a (a is the value of
	the current pixel) can be simplified with partial sums of the freq vector
	*/
	int *partial_sum = (int *)calloc(max_val_pixel + 1, sizeof(int));
	if (!partial_sum) {
		printf("Failed to calloc partial_sum (commands.c -> equalize\n");
		return;
	}

	partial_sum[0] = freq[0];
	for (int i = 1; i <= max_val_pixel; ++i)
		partial_sum[i] = partial_sum[i - 1] + freq[i];

	for (int i = 0; i < lin; ++i)
		for (int j = 0; j < col; ++j)
			new_arr[i][j].value = round(max_val_pixel * 1.0 / (lin * col) *
										partial_sum[(int)(*arr)[i][j].value]);

	clamp(&new_arr, lin, col, max_val_pixel);

	free_arr(arr, lin);
	*arr = new_arr;

	printf("Equalize done\n");

	free(freq);
	free(partial_sum);
}

static void rotate_right(pixel ***arr, area *active, int *lin, int *col,
						 int full_img)
{
	int lin_aux = *lin, col_aux = *col;
	if (full_img) {
		lin_aux = *col;
		col_aux = *lin;
	}

	pixel **new_arr = (pixel **)malloc(lin_aux * sizeof(pixel *));
	if (!new_arr) {
		printf("Failed to malloc new_arr (commands.c -> rotate_right)\n");
		return;
	}
	for (int i = 0; i < lin_aux; ++i) {
		new_arr[i] = (pixel *)calloc(col_aux, sizeof(pixel));
		if (!new_arr) {
			printf("Failed to calloc new_arr's columns (commands.c -> "
				   "rotate_right)\n");
			return;
		}
	}

	/*
	We repopulate the new array of pixels with the corresponding values,
	where i, j are used in reference to the new array and a, b are used for
	the old array (pre-rotate array)
	*/
	for (int i = 0; i < lin_aux; ++i)
		for (int j = 0; j < col_aux; ++j) {
			// Default case, if the current pixel isn't in the selection, it
			// doesn't get rotated
			int pre_rotate_lin = i, pre_rotate_col = j;

			/*
			If the whole image gets rotated:
				- the pre-rotate line is equal to the complement of the current
				  column to the total number of columns (pre-rotate number of
				  lines)
				- the pre-rotate column is equal to the current line
			*/
			if (full_img) {
				pre_rotate_lin = col_aux - j - 1;
				pre_rotate_col = i;
			} else if (in_selection(i, j, active)) {
				/*
				If only a portion of the image rotates, same rules as above
				apply but only to the selected area. We treat the area as a
				separate square, we find the absolute relative values and add
				them to the coords of the top-left corner of the selected area
				*/
				int num_of_col = (*active).x2 - (*active).x1;
				int i_abs = i - (*active).y1;
				int j_abs = j - (*active).x1;

				int i_sol = num_of_col - j_abs - 1;
				int j_sol = i_abs;

				pre_rotate_lin = (*active).y1 + i_sol;
				pre_rotate_col = (*active).x1 + j_sol;
			}

			new_arr[i][j] = (*arr)[pre_rotate_lin][pre_rotate_col];
		}

	free_arr(arr, *lin);
	*arr = new_arr;
	*lin = lin_aux;
	*col = col_aux;

	if (full_img) {
		int aux = (*active).x2;
		(*active).x2 = (*active).y2;
		(*active).y2 = aux;
	}
}

void rotate(pixel ***arr, area *active, int *lin, int *col)
{
	char *param = (char *)calloc(STR_SIZE, sizeof(char));
	if (!param) {
		printf("Failed to calloc param (commands.c -> select_area)\n");
		free(param);
		return;
	}

	read_param(&param);
	int angle = atol(param);
	if (angle == 0 && param[0] != '0') {
		printf("Invalid command\n");
		free(param);
		return;
	}
	free(param);

	// Stores in 'full_img' if we are rotating the whole image or just an area
	int full_img = (!(*active).x1 && !(*active).y1 && (*active).x2 == *col &&
					(*active).y2 == *lin);
	if (!full_img &&
		(*active).x2 - (*active).x1 != (*active).y2 - (*active).y1) {
		printf("The selection must be square\n");
		return;
	}

	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}

	int current_angle = angle;
	while (current_angle % 360 != 0) {
		rotate_right(arr, active, lin, col, full_img);
		current_angle -= 90;
	}

	printf("Rotated %d\n", angle);
}

void crop(pixel ***arr, area *active, int *lin, int *col)
{
	int lin_aux = (*active).y2 - (*active).y1;
	int col_aux = (*active).x2 - (*active).x1;

	pixel **new_arr = (pixel **)malloc(lin_aux * sizeof(pixel *));
	if (!new_arr) {
		printf("Failed to malloc aux (commands.c -> crop)\n");
		return;
	}
	for (int i = 0; i < lin_aux; ++i) {
		new_arr[i] = (pixel *)malloc(col_aux * sizeof(pixel));
		if (!new_arr) {
			printf("Failed to malloc aux's columns (commands.c -> crop)\n");
			return;
		}
	}

	for (int i = 0; i < lin_aux; ++i)
		for (int j = 0; j < col_aux; ++j)
			new_arr[i][j] = (*arr)[i + (*active).y1][j + (*active).x1];

	free_arr(arr, *lin);
	*arr = new_arr;

	*lin = lin_aux;
	*col = col_aux;

	(*active).x1 = (*active).y1 = 0;
	(*active).x2 = *col;
	(*active).y2 = *lin;

	printf("Image cropped\n");
}

static void apply_filter(pixel ***arr, double filter[3][3], area *active,
						 int lin, int col, int max_val_pixel)
{
	pixel **aux = (pixel **)malloc(lin * sizeof(pixel *));
	if (!aux) {
		printf("Failed to malloc aux (commands.c -> apply_filter)\n");
		return;
	}
	for (int i = 0; i < lin; ++i) {
		aux[i] = (pixel *)calloc(col, sizeof(pixel));
		if (!aux) {
			printf("Failed to calloc aux's columns (commands.c -> "
				   "apply_filter)\n");
			return;
		}
	}

	// If the pixel isn't in the selected area or is on the border of the image
	// it remains unedited
	for (int i = 0; i < lin; ++i)
		for (int j = 0; j < col; ++j)
			if (!in_selection(i, j, active) || on_border(i, j, lin, col))
				aux[i][j] = (*arr)[i][j];

	// Stores the border of the area on which a filter can be applied, for ease
	// of use
	int x1 = max((*active).x1, 1);
	int x2 = min((*active).x2, col - 1);
	int y1 = max((*active).y1, 1);
	int y2 = min((*active).y2, lin - 1);

	// Applies the filter on the area defined by the previously defined
	// variables
	for (int i = y1; i < y2; ++i)
		for (int j = x1; j < x2; ++j)
			for (int a = 0; a < 3; ++a)
				for (int b = 0; b < 3; ++b) {
					aux[i][j].r +=
						filter[a][b] * (*arr)[i + a - 1][j + b - 1].r;
					aux[i][j].g +=
						filter[a][b] * (*arr)[i + a - 1][j + b - 1].g;
					aux[i][j].b +=
						filter[a][b] * (*arr)[i + a - 1][j + b - 1].b;
				}

	clamp(&aux, lin, col, max_val_pixel);

	free_arr(arr, lin);

	*arr = aux;
}

void apply(pixel ***arr, area *active, int lin, int col, int img_type,
		   char *str, int max_val_pixel)
{
	// If there is nothing after the APPLY command, it's invalid
	if (!str[6]) {
		printf("Invalid command\n");
		return;
	}

	char *param = (char *)calloc(STR_SIZE, sizeof(char));
	if (!param) {
		printf("Failed to calloc param (commands.c -> select_area)\n");
		return;
	}

	read_param(&param);

	if (img_type != 2) {
		printf("Easy, Charlie Chaplin\n");
		free(param);
		return;
	}

	if (strcmp(param, "EDGE") == 0) {
		double filter[3][3] = {
			{-1.0, -1.0, -1.0},
			{-1.0,  8.0, -1.0},
			{-1.0, -1.0, -1.0},
		};

		apply_filter(arr, filter, active, lin, col, max_val_pixel);
		printf("APPLY EDGE done\n");
	} else if (strcmp(param, "SHARPEN") == 0) {
		double filter[3][3] = {
			{ 0.0, -1.0,  0.0},
			{-1.0,  5.0, -1.0},
			{ 0.0, -1.0,  0.0},
		};

		apply_filter(arr, filter, active, lin, col, max_val_pixel);
		printf("APPLY SHARPEN done\n");
	} else if (strcmp(param, "BLUR") == 0) {
		double filter[3][3] = {
			{1.0 / 9, 1.0 / 9, 1.0 / 9},
			{1.0 / 9, 1.0 / 9, 1.0 / 9},
			{1.0 / 9, 1.0 / 9, 1.0 / 9},
		};

		apply_filter(arr, filter, active, lin, col, max_val_pixel);
		printf("APPLY BLUR done\n");
	} else if (strcmp(param, "GAUSSIAN_BLUR") == 0) {
		double filter[3][3] = {
			{1.0 / 16, 2.0 / 16, 1.0 / 16},
			{2.0 / 16, 4.0 / 16, 2.0 / 16},
			{1.0 / 16, 2.0 / 16, 1.0 / 16},
		};

		apply_filter(arr, filter, active, lin, col, max_val_pixel);
		printf("APPLY GAUSSIAN_BLUR done\n");
	} else {
		printf("APPLY parameter invalid\n");
	}

	free(param);
}

void save(pixel **arr, int img_type, int lin, int col, int max_val_pixel)
{
	char *param = (char *)calloc(STR_SIZE, sizeof(char));
	if (!param) {
		printf("Failed to calloc param (commands.c -> select_area)\n");
		return;
	}

	read_param(&param);
	char *file_name = strdup(param);

	FILE *image;
	read_param(&param);

	if (strcmp(param, file_name) == 0)
		image = fopen(file_name, "wb");
	else
		image = fopen(file_name, "wt");

	/*
	we store img_type as 0, 1, 2 while the magic word is stored from 1 through
	6, therefore we add 1. Also if the file type is ascii the values are 1, 2, 3
	and binary has values 4, 5, 6, therefore if the file is binary we add 3.
	*/
	img_type += 1 + (strcmp(param, file_name) == 0) * 3;
	// Magic word
	fprintf(image, "P%d\n", img_type);
	// Lines, columns, and maximum value for pixels
	fprintf(image, "%d %d\n%d\n", col, lin, max_val_pixel);
	// Pixel values
	for (int i = 0; i < lin; ++i) {
		for (int j = 0; j < col; ++j) {
			if (img_type % 3 == 0) {
				if (img_type > 3) {
					unsigned char red, green, blue;
					red = arr[i][j].r;
					green = arr[i][j].g;
					blue = arr[i][j].b;
					fwrite(&red, sizeof(char), 1, image);
					fwrite(&green, sizeof(char), 1, image);
					fwrite(&blue, sizeof(char), 1, image);
				} else {
					fprintf(image, "%d %d %d ", (int)arr[i][j].r,
							(int)arr[i][j].g, (int)arr[i][j].b);
				}
			} else {
				if (img_type > 3) {
					unsigned char value = arr[i][j].value;
					fwrite(&value, sizeof(char), 1, image);
				} else {
					fprintf(image, "%d ", (int)arr[i][j].value);
				}
			}
		}

		if (img_type <= 3)
			fprintf(image, "\n");
	}

	printf("Saved %s\n", file_name);

	fclose(image);
	if (param != file_name)
		free(file_name);
	free(param);
}

#endif // COMMANDS_H
