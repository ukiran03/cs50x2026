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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            int r = image[i][j].rgbtRed;

            // Calculate sepia values using the provided algorithm
            int sepiaRed = round((0.393 * r) + (0.769 * g) + (0.189 * b));
            int sepiaGreen = round((0.349 * r) + (0.686 * g) + (0.168 * b));
            int sepiaBlue = round((0.272 * r) + (0.534 * g) + (0.131 * b));

            // Cap the values at 255 if they overflow
            if (sepiaRed > 255) {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255) {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255) {
                sepiaBlue = 255;
            }

            // Update pixel values
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
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
