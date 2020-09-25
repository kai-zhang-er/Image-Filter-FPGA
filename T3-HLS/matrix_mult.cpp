
#include "matrix_mult.h"

void matrix_mult(
	    mat image[IMAGE_ROWS][IMAGE_COLS],
	    mat filter[FILTER_ROWS][FILTER_COLS],
	    mat_prod prod[IMAGE_ROWS][IMAGE_COLS])
{
	char PaddingRows=floor(FILTER_ROWS/2);
	char PaddingCols=floor(FILTER_COLS/2);

	FirstPadRow:for (int i=0;i<PaddingRows;i++)
	{
		for(int j=0;j<IMAGE_COLS;j++)
		{
			prod[i][j]=0;
		}
	}
	LastPadRow:for (int i=IMAGE_ROWS-PaddingRows;i<IMAGE_ROWS;i++)
	{
		for(int j=0;j<IMAGE_COLS;j++)
		{
			prod[i][j]=0;
		}
	}
	FirstPadCol:for (int i=PaddingRows;i<IMAGE_ROWS-PaddingRows;i++)
	{
		for(int j=0;j<PaddingCols;j++)
		{
			prod[i][j]=0;
		}
	}
	LastPadCol:for (int i=PaddingRows;i<IMAGE_ROWS-PaddingRows;i++)
	{
		for(int j=IMAGE_COLS-PaddingCols;j<IMAGE_COLS;j++)
		{
			prod[i][j]=0;
		}
	}

	// Iterate over the rows of the image
	Row: for(int i = PaddingRows; i < IMAGE_ROWS-PaddingRows; i++)
	{
	  // Iterate over the columns of the B matrix
		Col: for(int j = PaddingCols; j < IMAGE_COLS-PaddingCols; j++)
		{
			prod[i][j]=0;
			for(int m=-1;m<2;m++)
			{
				for(int n=-1;n<2;n++)
				{
					prod[i][j]+=image[i+m][j+n];
				}
			}
			prod[i][j]/=FILTER_ROWS*FILTER_COLS;
		}
	}
}

