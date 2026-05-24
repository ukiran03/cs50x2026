#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

static const u32 BLOCK_SIZE = 512;

int main(int argc, char *argv[]) {
    // Accept a single command-line argument
    if (argc != 2) {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL) {
        printf("Could not open input file.\n");
        return 1;
    }

    u8 buffer[BLOCK_SIZE];
    int jpg_count = 0;

    FILE *output = NULL;
    char filename[8]; // big enough to hold "000.jpg\0"

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE) {

        // check if this block marks the start of a new JPEG
        bool is_jpg_header = buffer[0] == 0xff && buffer[1] == 0xd8 &&
                             buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;

        if (is_jpg_header) {
            // If a JPEG file is already open, close it before starting a new
            // one
            if (output != NULL) {
                fclose(output);
            }

            // generate the new filename and open the file
            sprintf(filename, "%03i.jpg", jpg_count);
            output = fopen(filename, "wb");
            if (output == NULL) {
                printf("Could not open output file.\n");
                fclose(card);
                return 1;
            }

            jpg_count++;
        }

        // If we have an open JPEG file, write the current block to it
        if (output != NULL) {
            fwrite(buffer, 1, BLOCK_SIZE, output);
        }
    }

    // Close any remaining open files
    if (output != NULL) {
        fclose(output);
    }
    fclose(card);

    return 0;
}
