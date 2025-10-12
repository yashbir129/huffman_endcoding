#include "core.h"

void encode_file(const char *input, const char *output) {
    int freq[256];
    get_frequency(input, freq);

    Node *root = build_huffman_tree(freq);

    FILE *in = fopen(input, "rb");
    FILE *out = fopen(output, "wb");
    if (!in || !out) {
        perror("File open error");
        return;
    }

    int c;
    while ((c = fgetc(in)) != EOF)
        fputc(c, out);  // placeholder (actual encoding logic goes here)

    fclose(in);
    fclose(out);
    free_tree(root);
}