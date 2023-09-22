#ifndef STRUCT_H
#define STRUCT_H

/*
Black & white, and grayscale both need only one value for each pixel so we can
use 'value' to represent both, and 'r', 'g', 'b' for colors
*/
typedef struct _pixel {
	double value;
	double r, g, b;
} pixel;

/*
Represents the coordinates of the top-left and bottom-right corners of the
area we are currently working on (x is for columns, y is for lines)
*/
typedef struct _area {
	int x1, y1, x2, y2;
} area;

#endif // STRUCT_H
