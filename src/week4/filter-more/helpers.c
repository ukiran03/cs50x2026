#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    // Loop over all pixels
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            // Take average of red, green, and blue using 3.0 to prevent
            // integer division
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen +
                             image[i][j].rgbtRed) /
                            3.0);

            // Update pixel values with the average to make it grayscale
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
    for (int r = 0; r < height; r++) {
        // i starts at the left, j starts at the right, meeting in the middle
        for (int i = 0, j = width - 1; i < j; i++, j--) {

            RGBTRIPLE temp = image[r][i];
            image[r][i] = image[r][j];
            image[r][j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            copy[i][j] = image[i][j];
        }
    }

    // NOTE: The method here is often called a 'kernel sum' or 'box blur'
    // mechanism.
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int pixel_count = 0;

            // check the 3x3 neighborhood around image[r][c]
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int neighborRow = r + dr;
                    int neighborCol = c + dc;

                    // boundary check to handle edges and corners safely
                    if (neighborRow >= 0 && neighborRow < height &&
                        neighborCol >= 0 && neighborCol < width) {

                        sumRed += copy[neighborRow][neighborCol].rgbtRed;
                        sumGreen += copy[neighborRow][neighborCol].rgbtGreen;
                        sumBlue += copy[neighborRow][neighborCol].rgbtBlue;

                        pixel_count++;
                    }
                }
            }
            image[r][c].rgbtRed = round(sumRed / (float)pixel_count);
            image[r][c].rgbtGreen = round(sumGreen / (float)pixel_count);
            image[r][c].rgbtBlue = round(sumBlue / (float)pixel_count);
        }
    }
}

int sobel_calc(int x, int y);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]) {
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            copy[i][j] = image[i][j];
        }
    }

    int gx_kernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy_kernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            int GxRed = 0, GyRed = 0;
            int GxGreen = 0, GyGreen = 0;
            int GxBlue = 0, GyBlue = 0;

            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int imgRow = r + dr, imgCol = c + dc;
                    int kernelRow = 1 + dr, kernelCol = 1 + dc;

                    // Default values for out of bound pixels (black border)
                    int red = 0, green = 0, blue = 0;

                    if (imgRow >= 0 && imgRow < height && imgCol >= 0 &&
                        imgCol < width) {
                        red = copy[imgRow][imgCol].rgbtRed;
                        green = copy[imgRow][imgCol].rgbtGreen;
                        blue = copy[imgRow][imgCol].rgbtBlue;
                    }
                    int gx_red = red * gx_kernel[kernelRow][kernelCol];
                    int gy_red = red * gy_kernel[kernelRow][kernelCol];
                    GxRed += gx_red;
                    GyRed += gy_red;

                    int gx_green = green * gx_kernel[kernelRow][kernelCol];
                    int gy_green = green * gy_kernel[kernelRow][kernelCol];
                    GxGreen += gx_green;
                    GyGreen += gy_green;

                    int gx_blue = blue * gx_kernel[kernelRow][kernelCol];
                    int gy_blue = blue * gy_kernel[kernelRow][kernelCol];
                    GxBlue += gx_blue;
                    GyBlue += gy_blue;
                }
            }

            image[r][c].rgbtRed = sobel_calc(GxRed, GyRed);
            image[r][c].rgbtGreen = sobel_calc(GxGreen, GyGreen);
            image[r][c].rgbtBlue = sobel_calc(GxBlue, GyBlue);
        }
    }
    return;
}

// Calculates the Sobel filter value: round(sqrt(x^2 + y^2)), capped at 255
int sobel_calc(int x, int y) {
    double combined = sqrt((double)(x * x) + (double)(y * y));

    int val = round(combined);

    return (val > 255) ? 255 : val;
}
