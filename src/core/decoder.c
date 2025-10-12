#include "core.h"

void decode_file(const char *input, const char *output) {
    FILE *in = fopen(input, "rb");
    FILE *out = fopen(output, "wb");
    if (!in || !out) {
        perror("File open error");
        return;
    }

    int c;
    while ((c = fgetc(in)) != EOF)
        fputc(c, out); // placeholder (actual decoding logic goes here)

    fclose(in);
    fclose(out);
}