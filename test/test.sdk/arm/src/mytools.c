/*
 * mytools.c
 *
 *  Created on: Dec 2, 2020
 *      Author: ck
 */

#include "mytools.h"

Mat** CreateMat(short r, short c)
{
	Mat** m;
	m = (Mat**)malloc(sizeof(Mat*) * r);

    for(short i = 0; i < r; i++)
	{
        m[i] = (Mat*) malloc(sizeof(Mat) * c);
    }
	return m;
}

Mat** Preprocessing(Mat image[IMAGE_ROWS][IMAGE_COLS][CHANNELS])
{
	Mat** gray=CreateMat(IMAGE_ROWS, IMAGE_COLS);
	for(int i=0;i<IMAGE_ROWS;i++)
	{
		for (int j=0;j<IMAGE_COLS;j++)
		{
			short temp=(image[i][j][0]+image[i][j][1]+image[i][j][2]);
			gray[i][j]=(Mat)(temp/3);
		}
	}

	return gray;
}

Mat** filter(Mat** src, short kernel[3][3])
{
	Mat** result=CreateMat(IMAGE_ROWS,IMAGE_COLS);
	char PaddingRows=1;
	char PaddingCols=1;
	for(int i=0;i<IMAGE_ROWS;i++)
	{
		for (int j=0;j<IMAGE_COLS;j++)
		{
			result[i][j]=0;
		}
	}

	for(int i = PaddingRows; i < IMAGE_ROWS-PaddingRows; i++)
	{
	  // Iterate over the columns of the B matrix
		for(int j = PaddingCols; j < IMAGE_COLS-PaddingCols; j++)
		{
			short temp=0;
			for(int m=-1;m<2;m++)
			{
				for(int n=-1;n<2;n++)
				{
					temp+=src[i+m][j+n]*kernel[1+m][1+n];
				}
			}
			result[i][j]=(Mat)(temp/9);
		}
	}
	return result;
}

Mat** derivateY(Mat** src)
{
	short kernel[3][3]={{-1,-1,-1},{0,0,0},{1,1,1}};
	Mat** result=filter(src,kernel);
	return result;
}

Mat** derivateX(Mat** src)
{
	short kernel[3][3]={{-1,0,1},{-1,0,1},{-1,0,1}};
	Mat** result=filter(src,kernel);
	return result;
}

Mat** Range(Mat** src, short startRow, short endRow,short startCol,short endCol)
{
	Mat** result=CreateMat(endRow-startRow,endCol-startCol);
	for(int i=startRow;i<endRow;i++)
	{
		for(int j=startCol;j<endCol;j++)
		{
			result[i-startRow][j-startCol]=src[i][j];
		}
	}
	return result;
}
Mat* searchMaxInterest(Mat** winGx,Mat** winGy)
{
	float selfCovMat[2][2];
	float I[WINDOW_SIZE][WINDOW_SIZE];
	for (int r = 0; r < WINDOW_SIZE; r++)
	{
		for (int c = 0; c < WINDOW_SIZE; c++)
		{
			//mutiply 1000 to avoid oversize
			selfCovMat[0][0] = 10.0*(float)winGx[r][c];
			selfCovMat[0][1] = 10.0*(float)winGx[r][c]*(float)winGy[r][c];
			selfCovMat[1][0] = selfCovMat[0][1];
			selfCovMat[1][1] = 10.0*(float)winGy[r][c];

			//calculate interest value
			float k = 0.5f;
			I[r][c] = abs(selfCovMat[0][0] * selfCovMat[1][1] - selfCovMat[0][1] * selfCovMat[1][0] - k*(selfCovMat[0][0] + selfCovMat[1][1])*(selfCovMat[0][0] + selfCovMat[1][1]));

		}
	}

	float maxI = 1;
	Mat* corner=(Mat*)malloc(sizeof(Mat)*2);
	for (int r = 0; r < WINDOW_SIZE; r++)
	{
		for (int c = 0; c < WINDOW_SIZE; c++)
		{
			if (I[r][c]>maxI)
			{
				maxI = I[r][c];
				corner[0] = c;
				corner[1] = r;
			}
		}
	}
	return corner;
}

Mat** Harris(Mat** preImage)
{
	Mat** deriX = derivateX(preImage);
	Mat** deriY = derivateY(preImage);
	short gaussianKernel[3][3]={{1,1,1},{1,1,1},{1,1,1}};
	deriX=filter(deriX, gaussianKernel);
	deriY=filter(deriY, gaussianKernel);

	Mat** resultPt=CreateMat(NUM_POINT,2);
	short ind=0;
	short stride=WINDOW_SIZE-1;
	for(int r=0;r<IMAGE_ROWS-WINDOW_SIZE;r+=stride)
	{
		for(int c=0;c<IMAGE_COLS-WINDOW_SIZE;c+=stride)
		{
			int edgeRow = min(IMAGE_ROWS, r + WINDOW_SIZE);
			int edgeCol = min(IMAGE_ROWS, c + WINDOW_SIZE);
			Mat** windowGx = Range(deriX,r, edgeRow,c, edgeCol);
			Mat** windowGy = Range(deriY,r, edgeRow,c, edgeCol);
			Mat* corner = searchMaxInterest(windowGx, windowGy);
			if (corner[0] < 0) continue;

			if(ind<NUM_POINT)
			{
				resultPt[ind][0]=corner[0]+c;
				resultPt[ind][1]=corner[1]+c;
				ind++;
			}
			else
			{
				return resultPt;
			}

		}
	}
	return resultPt;
}

Mat* CalculateDifference(Mat** preImage,Mat** prePt,Mat** nextImage,Mat** nextPt)
{
	// calculate difference in a local window
	Mat* minDiffInd=(Mat*)malloc(sizeof(Mat)*2);

	short minDiff=100;

	for( int i=0;i<NUM_POINT;i++)
	{
		for(int j=0;j<NUM_POINT;j++)
		{
			short difference=0;
			for (int r=-1;r<2;r++)
			{
				for (int c=-1;c<2;c++)
				{
					difference+=preImage[prePt[i][1]][prePt[i][0]]-nextImage[nextPt[i][1]][nextPt[i][0]];
				}
			}
			if (difference<minDiff)
			{
				minDiff=difference;
				minDiffInd[0]=i;
				minDiffInd[1]=j;
			}

		}
	}
	return minDiffInd;
}

