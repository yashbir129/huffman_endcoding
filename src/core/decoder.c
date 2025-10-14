<<<<<<< HEAD
/*
  decoder.c
  Usage: ./decoder input.huff output.txt
*/

#include "core.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern uint64_t read_header_and_freq(FILE *in, uint64_t freq_table[]);

extern Node* build_huffman_tree(uint64_t freq_table[]);

int decode_file(const char *inpath, const char *outpath) {
    FILE *in = fopen(inpath, "rb");
    if (!in) { fprintf(stderr, "Cannot open %s\n", inpath); return 1; }

    uint64_t freq_table[MAX_CHARS];
    uint64_t original_size = read_header_and_freq(in, freq_table);
    if (original_size == (uint64_t)-1) {
        fprintf(stderr, "Invalid or corrupt header.\n");
        fclose(in);
        return 1;
    }

    Node *root = build_huffman_tree(freq_table);
    if (!root) { // empty file case
        FILE *out = fopen(outpath, "wb");
        fclose(out);
        fclose(in);
        return 0;
    }

    FILE *out = fopen(outpath, "wb");
    if (!out) { fprintf(stderr, "Cannot open output\n"); fclose(in); free_huffman_tree(root); return 1; }

    // read bitstream and traverse tree
    Node *curr = root;
    uint64_t written = 0;
    int b;
    while ((b = fgetc(in)) != EOF && written < original_size) {
        unsigned char byte = (unsigned char)b;
        for (int i = 0; i < 8 && written < original_size; ++i) {
            int bit = (byte & (1 << (7 - i))) ? 1 : 0;
            curr = bit ? curr->right : curr->left;
            if (!curr->left && !curr->right) {
                fputc(curr->ch, out);
                written++;
                curr = root;
            }
        }
    }

    fclose(in);
    fclose(out);
    free_huffman_tree(root);
    return 0;
}

#ifdef STANDALONE_DECODER
int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.huff output.txt\n", argv[0]);
        return 1;
    }
    return decode_file(argv[1], argv[2]);
}
#endif
=======
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
>>>>>>> f98685467c920b6d7d448c00b85ec85a17a3aa9b
