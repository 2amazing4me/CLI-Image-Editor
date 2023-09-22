// Copyright 2023 Baldovin Razvan-Mihai-Marian 312CA

#include <stdio.h>

#include "handler.h"
#include "struct.h"

int main(void)
{
	pixel **arr;
	area active;

	/*
	img_type will be one of the following: - 0, if black and white
										   - 1, if grayscale
										   - 2, if color
	*/
	int img_type;
	int lin, col;
	int max_val_pixel;

	task_processor(&arr, &active, &img_type, &lin, &col, &max_val_pixel);

	return 0;
}
