## Baldovin Razvan-Mihai-Marian 312CA

# Overview

The program is a very reduced and simplified version of image-editing software
such as Adobe Photoshop. It works with .ppm and .pgm formats of images and can 
do some basic commands to edit the whole picture or an area of it.

================================================================================

# Files

The program's functionality is split into 4 separate .c files with 4 .h header
files (each has it's own, apart from main.c, and there is one header file for 
the struct's definitions). As their names suggest, they only hold functions that
fall into one of four general purposes:
    - 'main.c' - initializes the variables and starts the main loop.
    - 'handler.c' - contains the main loop which handles each command.
    - 'commands.c' - contains all the possible commands that can be executed
    - 'helpers.c' - contains all miscellaneous functions which are created to
                    aid in writing more concise and readable code
    - 'struct.h' - contains the definition for types 'pixel' and 'area' to be
                   used in the program

================================================================================

# Structs

To simplify the code, there are 2 structures designed to help add readability
and minimize the number of separate variables, in order to make code shorter

Pixel: - (double) r, g, b - values used to store color pixels (Red, Green, Blue)
       - (double) value - value used to store grayscale pixels

Area: - (int) x1, y1, x2, y2 - values used to store the the top-left corner
                               (x1, y1) and bottom-right corner (x2, y2) of the
                               selection

================================================================================

# Variables

The program revolves around a couple of important variables that define the
image we are working on:
    - (pixel **) arr - stores the image as an array of pixels
    - (area) active - stores the currently selected area of the picture
    - (int) lin, col - store the dimensions of the whole picture
    - (int) img_type - stores the type of image (grayscale or color)
    - (int) max_val_pixel - stores the maximum value for a pixel

================================================================================

# Functions

## handler.c

### read_command
Reads the command from input 

### task_processor
The exoskeleton of the entire program, handles input and each command 
accordingly

## commands.c

### load_file
Reads the location of the image relative to the project and imports the type of
image, the size of the picture, the max value of pixels and the values of each
pixel

### select_area
Selects an area of the image for future commands

### histogram
Creates the histogram of the selected area in the image, with as many bins and 
stars as the user wants (grayscale only)

### equalize
Edits the image in such a way that it flattens the picture's histogram
(grayscale only)

### rotate_right (static)
Rotates the given image or area within an image, 90 degrees clockwise once

### rotate
Handles the rotation of an image or area within an image at -/+ 90, -/+ 180, 
-/+ 270 and -/+ 360 degrees

### crop
Modifies the image to only contain the selected area from within

### apply_filter (static)
Applies the current filter(kernel) to the selected area of an image

### apply
Handles the applinace of one of four (EDGE, SHARPEN, BLUR, GAUSSIAN_BLUR) 
filters to an image (color only)

### save 
Saves the current array as an image in the location given as parameter, either
as text or binary (specified by the presence of the parameter 'ascii')

## helpers.c

### trim
Restrains the current token to their exact value, without the newline character

### read_param
Reads the next parameter of the previously entered command

### check_image_loaded
Checks if there is currently a loaded image

### check_coords
Checks if the given coordinates can describe a valid area of the image

### check_param
Checks if there exists a parameter. If the parameter exists, it also checks that
it is a number

### min, max
Returns the minimum/maximum of two numbers

### clamp
Returns: - 0, if the value is smaller than 0
         - max_val_pixel, if the value is bigger than the maximum value for 
                          pixels
         - value, if the value is between 0 and max_val_pixel

### in_selection
Checks if the current position is in the selected area

### on_border
Checks if the current position is on the border of the image

### free_arr
Frees the array

================================================================================

# Usage

Each command is represented by the name of the command itself (in upper case) 
and, for some commands, some parameters.

Here is a short list with each command and what it does:
    - LOAD <file_name>
        - Loads the file given as parameter
    - SELECT <x1> <y1> <x2> <y2>
        - Selects the area defined by the parameters 
    - SELECT ALL
        - Selects the whole image
    - HISTOGRAM <stars> <bins>
        - Creates the histogram with as many stars and bins as specified for
        the given selection
    - EQUALIZE
        - Equalizes the whole image
    - CROP
        - Crops the image to only contain the selected size
    - APPLY <filter>
        - <filter> can be: - <EDGE>
                           - <SHARPEN>
                           - <BLUR>
                           - <GAUSSIAN_BLUR>
        - Applies the given filter to the selected area
    - SAVE <file_name> [ascii]
        - Saves the current array as a picture named <file_name> which is:
            - text, if the ascii parameter is specified
            - binary, otherwise
    - EXIT
        - Gracefully exits the program, freeing everything
    (BONUS)
    - ROTATE <angle>
        - Rotates the selected area or image <angle> degrees

================================================================================

# Feedback

Great homework, very fun, very educative