#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the functions before including the source files
int encode_file(const char *inpath, const char *outpath);
int decode_file(const char *inpath, const char *outpath);

// Include the source files directly
#include "src/core/core.h"
#include "src/core/encoder.c"
#include "src/core/decoder.c"
#include "src/core/frequency.c"
#include "src/core/huffman_tree.c"
#include "src/core/utils.c"

int main(int argc, char **argv) {
    if (argc < 4) {
        printf("Usage: %s [encode|decode] input_file output_file\n", argv[0]);
        return 1;
    }

    const char *mode = argv[1];
    const char *input_file = argv[2];
    const char *output_file = argv[3];

    if (strcmp(mode, "encode") == 0) {
        printf("Encoding %s to %s\n", input_file, output_file);
        return encode_file(input_file, output_file);
    } 
    else if (strcmp(mode, "decode") == 0) {
        printf("Decoding %s to %s\n", input_file, output_file);
        return decode_file(input_file, output_file);
    }
    else {
        printf("Unknown mode: %s\n", mode);
        printf("Usage: %s [encode|decode] input_file output_file\n", argv[0]);
        return 1;
    }
}