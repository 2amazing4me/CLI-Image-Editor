#include "struct.h"

void trim(char **str);

void read_param(char **param);

int check_image_loaded(int img_loaded);

int check_coords(int x1, int y1, int x2, int y2, int lin, int col);

int check_param(int val, char *param, char *str, int *pos);

double min(double a, double b);

double max(double a, double b);

void clamp(pixel ***arr, int lin, int col, int max_val_pixel);

int in_selection(int i, int j, area *active);

int on_border(int i, int j, int lin, int col);

void free_arr(pixel ***arr, int lin);
