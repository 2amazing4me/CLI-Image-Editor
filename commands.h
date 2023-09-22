#include "struct.h"

void load_file(pixel * **arr, area * active, int *img_type, int *lin, int *col,
			   int *max_val_pixel, int *img_loaded);

void select_area(area *active, int lin, int col, char *str);

void histogram(pixel **arr, area active, int img_type, int max_val_pixel,
			   char *str);

void equalize(pixel ***arr, int lin, int col, int img_type, int max_val_pixel);

void rotate(pixel ***arr, area *active, int *lin, int *col);

void crop(pixel ***arr, area *active, int *lin, int *col);

void apply(pixel ***arr, area *active, int lin, int col, int img_type,
		   char *str, int max_val_pixel);

void save(pixel **arr, int img_type, int lin, int col, int max_val_pixel);
