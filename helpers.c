#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float temp;
    // For each row of pixels
    for (int r = 0; r < height; r++)
    {
        // Dor each column of pixels
        for (int c = 0; c < width; c++)
        {
            // Calculate the average of the current pixel's colors.
            temp = round((image[r][c].rgbtBlue + image[r][c].rgbtGreen + image[r][c].rgbtRed) / 3.0);
            // Update the current pixel's values with its new values
            image[r][c].rgbtBlue = temp;
            image[r][c].rgbtGreen = temp;
            image[r][c].rgbtRed = temp;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    // For each row of pixels
    for (int r = 0; r < height; r++)
    {
        // For each column of pixels up to the "midpoint" of the image
        for (int c = 0; c < (width / 2); c++)
        {
            temp = image[r][c]; // save the current pixel
            image[r][c] = image[r][width - 1 - c]; // set the new current pixel to be the mirrored pixel
            image[r][width - 1 - c] = temp; // set the new mirrored pixel to be the previous current pixel
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Use a temp array to work with the original values of the image array
    RGBTRIPLE temp[height][width];
    // Copy the contents of the image array into the temp array
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            temp[r][c] = image[r][c];
        }
    }
    int sumBlueValues, sumGreenValues, sumRedValues, numPixels;
    // For each row of pixels
    for (int r = 0; r < height; r++)
    {
        // For each column of pixels
        for (int c = 0; c < width; c++)
        {
            // Reset the sum of each of the pixel's values
            sumBlueValues = 0;
            sumGreenValues = 0;
            sumRedValues = 0;
            numPixels = 0;
            // For each row of pixels of the 3x3 grid of the current pixel's neighboring pixels
            for (int i = r - 1; i <= r + 1; i++)
            {
                // For each column of pixels of the 3x3 grid of the current pixel's neighboring pixels
                for (int j = c - 1; j <= c + 1; j++)
                {
                    // If the current neighboring pixel is within the border of the image array
                    if (i >= 0 && i <= (height - 1) && j >= 0 && j <= (width - 1))
                    {
                        // update the sums with the current neighboring pixel's values and update the # of valid neighboring pixels
                        sumBlueValues += temp[i][j].rgbtBlue;
                        sumGreenValues += temp[i][j].rgbtGreen;
                        sumRedValues += temp[i][j].rgbtRed;
                        numPixels++;
                    }
                }
            }
            // Calculate the average for each color channel and round the average to the nearest integer
            double blueValuesAvg = round((sumBlueValues / (numPixels * 1.0)));
            double greenValuesAvg = round(sumGreenValues / (numPixels * 1.0));
            double redValuesAvg = round((sumRedValues / (numPixels * 1.0)));   
            // Update the current pixel's values with its new values
            image[r][c].rgbtBlue = (BYTE) blueValuesAvg;
            image[r][c].rgbtGreen = (BYTE) greenValuesAvg;
            image[r][c].rgbtRed = (BYTE) redValuesAvg;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Use a temp array to work with the original values of the image array
    RGBTRIPLE temp[height][width];
    // Copy the contents of the image array into the temp array
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            temp[r][c] = image[r][c];
        }
    }
    // Construct the two kernals
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int blueChanGx, blueChanGy, greenChanGx, greenChanGy, redChanGx, redChanGy;
    // For each row of pixels
    for (int r = 0; r < height; r++)
    {
        // For each column of pixels
        for (int c = 0; c < width; c++)
        {
            // Reset the Gx and Gy values for each color channel
            blueChanGx = 0;
            blueChanGy = 0;
            greenChanGx = 0;
            greenChanGy = 0;
            redChanGx = 0;
            redChanGy = 0;
            // For each row of pixels of the 3x3 grid of the current pixel's neighboring pixels
            for (int i = r - 1; i <= r + 1; i++)
            {
                // For each column of pixels of the 3x3 grid of the current pixel's neighboring pixels
                for (int j = c - 1; j <= c + 1; j++)
                {
                    // If the current neighboring pixel is within the border of the image array
                    if (i >= 0 && i <= (height - 1) && j >= 0 && j <= (width - 1))
                    {
                        // update the Gx and Gy values for each color channel using the kernals
                        blueChanGx += (temp[i][j].rgbtBlue * gx[i - (r - 1)][j - (c - 1)]);
                        blueChanGy += (temp[i][j].rgbtBlue * gy[i - (r - 1)][j - (c - 1)]);
                        greenChanGx += (temp[i][j].rgbtGreen * gx[i - (r - 1)][j - (c - 1)]);
                        greenChanGy += (temp[i][j].rgbtGreen * gy[i - (r - 1)][j - (c - 1)]);
                        redChanGx += (temp[i][j].rgbtRed * gx[i - (r - 1)][j - (c - 1)]);
                        redChanGy += (temp[i][j].rgbtRed * gy[i - (r - 1)][j - (c - 1)]);
                    }
                }
            }
            // Calculate the square root of (Gx^2 + Gy^2) for each color channel and round the pixel's new color value to the nearest integer
            double newBlueValue = round(sqrt((blueChanGx * blueChanGx) + (blueChanGy * blueChanGy)));
            double newGreenValue = round(sqrt((greenChanGx * greenChanGx) + (greenChanGy * greenChanGy)));
            double newRedValue = round(sqrt((redChanGx * redChanGx) + (redChanGy * redChanGy)));
            // Cap the pixel's new color value at 255 if it exceeds the value 255
            newBlueValue = (newBlueValue > 255) ? 255 : (BYTE) newBlueValue;
            newGreenValue = (newGreenValue > 255) ? 255 : (BYTE) newGreenValue;
            newRedValue = (newRedValue > 255) ? 255 : (BYTE) newRedValue;
            // Update the current pixel's color values to its new values
            image[r][c].rgbtBlue = newBlueValue;
            image[r][c].rgbtGreen = newGreenValue;
            image[r][c].rgbtRed = newRedValue;
        }
    }
    return;
}