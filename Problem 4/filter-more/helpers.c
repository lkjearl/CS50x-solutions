#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width/2; j++)
        {
            // make temp = start cell of row initially
            BYTE tempB = image[i][j].rgbtBlue;
            BYTE tempG = image[i][j].rgbtGreen;
            BYTE tempR = image[i][j].rgbtRed;
            if (tempB == image[i][width -j -1].rgbtBlue && tempG == image[i][width -j -1].rgbtGreen && tempR == image[i][width -j -1].rgbtRed)
            {
                return;
            }
            else
            {
                // turn left cell into right value
                image[i][j].rgbtBlue = image[i][width -j -1].rgbtBlue;
                image[i][j].rgbtGreen = image[i][width -j -1].rgbtGreen;
                image[i][j].rgbtRed = image[i][width -j -1].rgbtRed;
                // turn right cell into original left value (from temp)
                image[i][width -j -1].rgbtBlue = tempB;
                image[i][width -j -1].rgbtGreen = tempG;
                image[i][width -j -1].rgbtRed = tempR;
            }
        }
    }

    return;
}

// Blur images
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // make temp copy
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    //make blur and transfer edited cell to temp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float counter = 0;
            int sumB =0;
            int sumG =0;
            int sumR =0;

            // for the 3x3
            for (int x = (i-1); x <= (i+1); x++)
            {
                for (int y = (j-1); y <= (j+1); y++)
                {
                    if (x < 0 || y < 0 || x >= height || y >= width)
                    {
                        continue;
                    }
                    sumB += temp[x][y].rgbtBlue;
                    sumG += temp[x][y].rgbtGreen;
                    sumR += temp[x][y].rgbtRed;
                    counter++;
                }
            }
            image[i][j].rgbtBlue = round(sumB/counter);
            image[i][j].rgbtGreen = round(sumG/counter);
            image[i][j].rgbtRed = round(sumR/counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // copy
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // declar 3x3 array/matrix to store value to multiply from
    int gx [3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };
    int gy [3][3] = {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };

    //overall loop
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int sumBx = 0;
            int sumGx = 0;
            int sumRx = 0;

            int sumBy = 0;
            int sumGy = 0;
            int sumRy = 0;
            //store algorithm calc value
            int SorbelB = 0;
            int SorbelG = 0;
            int SorbelR = 0;


            for (int x = (i-1), a = 0; x <= (i+1); x++, a++)
            {
                for (int y = (j-1), b = 0; y <= (j+1); y++, b++)
                {
                    if (x < 0 || y < 0 || x >= height || y >= width)
                    {
                        continue;
                    }
                    sumBx += temp[x][y].rgbtBlue * gx[a][b];
                    sumGx += temp[x][y].rgbtGreen * gx[a][b];
                    sumRx += temp[x][y].rgbtRed * gx[a][b];

                    sumBy += temp[x][y].rgbtBlue * gy[a][b];
                    sumGy += temp[x][y].rgbtGreen * gy[a][b];
                    sumRy += temp[x][y].rgbtRed * gy[a][b];


                }
            }
            //sobel algo: GX^2+GY^2 calc
            SorbelB = round(sqrt((sumBx * sumBx) + (sumBy * sumBy)));
            SorbelG = round(sqrt((sumGx * sumGx) + (sumGy * sumGy)));
            SorbelR = round(sqrt((sumRx * sumRx) + (sumRy * sumRy)));

            //reflect the algo to original image
            //FYI
            // sign = (s[i] == '-') ? -1 : 1;
            // is equivalent to below
            // if (s[i] == '-') {sign = -1;}
            // else {sign = 1;}
            image[i][j].rgbtBlue = (SorbelB > 255) ? 255 : SorbelB;
            image[i][j].rgbtGreen = (SorbelG > 255) ? 255 : SorbelG;
            image[i][j].rgbtRed = (SorbelR > 255) ? 255 : SorbelR;
        }
    }
    return;
}
