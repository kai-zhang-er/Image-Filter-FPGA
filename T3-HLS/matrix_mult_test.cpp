#include <iostream>
#include "matrix_mult.h"

using namespace std;

int main(int argc, char **argv)
{
   mat image[10][10] = {
      {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
	  {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
	  {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
      {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
      {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
	  {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
	  {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
	  {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
	  {0, 0, 0, 250, 250, 250, 0, 0, 0, 0},
	  {0, 0, 0, 250, 250, 250, 0, 0, 0, 0}
   };
   mat filter[3][3] = {
      {1, 1, 1},
      {1, 1, 1},
      {1, 1, 1}
   };
   mat_prod true_result[10][10], pred_result[10][10];
   int error_count = 0;

   // Generate the expected result
   // Iterate over the rows of the image
   char PaddingRows=floor(FILTER_ROWS/2);
   char PaddingCols=floor(FILTER_COLS/2);
   for(int i = 0; i < IMAGE_ROWS; i++)
   {
	   // iterate over the cols of the image
      for(int j = 0; j < IMAGE_COLS; j++)
      {
    	  if(j<PaddingCols || j>=IMAGE_COLS-PaddingCols ||i<PaddingRows||i>=IMAGE_ROWS-PaddingRows)
		  {
    		  true_result[i][j] = 0;
			  continue;
		  }
    	  true_result[i][j]=0;
		  for(int m=i-PaddingRows;m<i+PaddingRows;m++)
		  {
			  for(int n=j-PaddingCols;n<j+PaddingCols;n++)
			  {
				  true_result[i][j]+=image[m][n];
			  }
		  }
		  true_result[i][j]/=FILTER_ROWS*FILTER_COLS;
      }
   }

#ifdef HW_COSIM
   // Run the Vivado HLS matrix filter
   matrix_mult(image, filter, pred_result);
#endif

   // Print product matrix
   for (int i = 0; i < IMAGE_ROWS; i++) {
      for (int j = 0; j < IMAGE_COLS; j++) {
#ifdef HW_COSIM
         // Check result of HLS vs. expected
         if (pred_result[i][j] != true_result[i][j]) {
            error_count++;
         }
#else
         cout << true_result[i][j];
#endif
      }
   }

#ifdef HW_COSIM
   if (error_count)
      cout << "TEST FAIL: " << error_count << "Results do not match!" << endl;
   else
      cout << "Test passed!" << endl;
#endif
   return error_count;
}

