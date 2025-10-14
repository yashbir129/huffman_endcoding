/*
  encoder.c
  Usage: ./encoder input.txt output.huff
  Writes a header (original file size, unique count, symbol+freq list), then compressed bitstream.
*/

#include "core.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int build_frequency_table_from_file(const char *path, uint64_t freq_table[]);

extern int write_header_with_freq(FILE *out, uint64_t original_size, uint64_t freq_table[]);

int encode_file(const char *inpath, const char *outpath) {
    uint64_t freq_table[MAX_CHARS];
    if (build_frequency_table_from_file(inpath, freq_table) != 0) {
        fprintf(stderr, "Failed to open input file: %s\n", inpath);
        return 1;
    }

    // get original size
    FILE *f = fopen(inpath, "rb");
    fseek(f, 0, SEEK_END);
    uint64_t original_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    Node *root = build_huffman_tree(freq_table);
    if (!root) {
        fprintf(stderr, "Nothing to encode (empty file).\n");
        fclose(f);
        return 1;
    }

    Code code_table[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; ++i) code_table[i].bits = NULL;
    char buffer[512];
    build_code_table(root, code_table, buffer, 0);

    FILE *out = fopen(outpath, "wb");
    if (!out) { fclose(f); free_huffman_tree(root); return 1; }

    // write header
    write_header_with_freq(out, original_size, freq_table);

    // Now write bitstream: read input again and write bits
    int bit_pos = 0;
    unsigned char out_byte = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        char *bits = code_table[(unsigned char)c].bits;
        for (int i = 0; bits[i] != '\0'; ++i) {
            if (bits[i] == '1') out_byte |= (1 << (7 - bit_pos));
            bit_pos++;
            if (bit_pos == 8) {
                fputc(out_byte, out);
                out_byte = 0;
                bit_pos = 0;
            }
        }
    }
    // flush remaining bits
    if (bit_pos != 0) {
        fputc(out_byte, out);
    }

    // cleanup
    for (int i = 0; i < MAX_CHARS; ++i) {
        if (code_table[i].bits) free(code_table[i].bits);
    }
    free_huffman_tree(root);
    fclose(f);
    fclose(out);
    return 0;
}

#ifdef STANDALONE_ENCODER
int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.txt output.huff\n", argv[0]);
        return 1;
    }
    return encode_file(argv[1], argv[2]);
}
#endif
