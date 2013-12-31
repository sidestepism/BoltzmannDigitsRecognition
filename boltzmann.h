// boltzmann.h
#pragma once

#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"


#define PIXELS 784
#define WIDTH 28
#define HEIGHT 28

// from image.h 
typedef struct image {
    unsigned short pixels[PIXELS];
    unsigned short label;
} * image_t;

image_t mk_image();
image_t mk_image_from_file(FILE* image_fp, FILE* label_fp);
void show_image(image_t image);
