// Copyright 2023 Baldovin Razvan-Mihai-Marian 312CA

#ifndef HANDLER_H
#define HANDLER_H

#define INPUT_SIZE 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "helpers.h"
#include "struct.h"

void read_command(char **str, char **command)
{
	fgets(*str, INPUT_SIZE, stdin);

	*command = strtok(*str, " ");
	trim(command);
}

void task_processor(pixel ***arr, area *active, int *img_type, int *lin,
					int *col, int *max_val_pixel)
{
	char *str, *command;
	int img_loaded = 0, ok = 1;

	str = (char *)calloc(INPUT_SIZE, sizeof(char));
	if (!str) {
		printf("Failed to calloc str (handler.c)\n");
		return;
	}

	while (ok) {
		read_command(&str, &command);

		if (strcmp(command, "LOAD") == 0) {
			if (img_loaded) {
				free_arr(arr, *lin);
				img_loaded = 0;
			}
			load_file(arr, active, img_type, lin, col, max_val_pixel,
					  &img_loaded);
		} else if (strcmp(command, "SELECT") == 0) {
			if (check_image_loaded(img_loaded))
				select_area(active, *lin, *col, str);
		} else if (strcmp(command, "HISTOGRAM") == 0) {
			if (check_image_loaded(img_loaded))
				histogram(*arr, *active, *img_type, *max_val_pixel, str);
		} else if (strcmp(command, "EQUALIZE") == 0) {
			if (check_image_loaded(img_loaded))
				equalize(arr, *lin, *col, *img_type, *max_val_pixel);
		} else if (strcmp(command, "ROTATE") == 0) {
			if (check_image_loaded(img_loaded))
				rotate(arr, active, lin, col);
		} else if (strcmp(command, "CROP") == 0) {
			if (check_image_loaded(img_loaded))
				crop(arr, active, lin, col);
		} else if (strcmp(command, "APPLY") == 0) {
			if (check_image_loaded(img_loaded))
				apply(arr, active, *lin, *col, *img_type, str, *max_val_pixel);
		} else if (strcmp(command, "SAVE") == 0) {
			if (check_image_loaded(img_loaded))
				save(*arr, *img_type, *lin, *col, *max_val_pixel);
		} else if (strcmp(command, "EXIT") == 0) {
			if (check_image_loaded(img_loaded))
				free_arr(arr, *lin);

			free(str);
			ok = 0;
		} else {
			printf("Invalid command\n");
		}
	}
}

#endif // HANDLER_H
