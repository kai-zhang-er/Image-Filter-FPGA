#ifndef __MATRIXMUL_H__
#define __MATRIXMUL_H__

#include <cmath>
using namespace std;

// Compare TB vs HW C-model and/or RTL
#define HW_COSIM

#define IMAGE_ROWS 10
#define IMAGE_COLS 10
#define FILTER_ROWS 3
#define FILTER_COLS 3


typedef unsigned char mat;
typedef short mat_prod;

// Prototype of top level function for C-synthesis
void matrix_mult(
      mat image[IMAGE_ROWS][IMAGE_COLS],
      mat filter[FILTER_ROWS][FILTER_COLS],
      mat_prod prod[IMAGE_ROWS][IMAGE_COLS]);

#endif // __MATRIXMUL_H__ not defined

