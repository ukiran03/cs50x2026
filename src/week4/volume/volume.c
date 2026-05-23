// Modifies the volume of an audio file

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

// Number of bytes in .wav header
static const u32 HEADER_SIZE = 44;

int main(int argc, char *argv[]) {
    // check command-line arguments
    if (argc != 4) {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL) {
        printf("Could not open input file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL) {
        printf("Could not open output file.\n");
        fclose(input); // Clean up already opened file
        return 1;
    }

    f32 factor = atof(argv[3]);

    // Copy header from input file to output file (EXACTLY ONCE)
    u8 header[HEADER_SIZE];
    if (fread(header, HEADER_SIZE, 1, input) == 1) {
        fwrite(header, HEADER_SIZE, 1, output);
    }

    // Read samples from input file and write updated data to output file

    // create a buffer for a single sample
    i16 buffer;

    while (fread(&buffer, sizeof(i16), 1, input) == 1) {
        buffer *= factor;

        fwrite(&buffer, sizeof(i16), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);

    return 0;
}
