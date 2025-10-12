#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    unsigned char ch;
    int freq;
    struct Node *left, *right;
} Node;

void get_frequency(const char *filename, int freq[]);
Node* build_huffman_tree(int freq[]);
void free_tree(Node *root);
void encode_file(const char *input, const char *output);
void decode_file(const char *input, const char *output);

#endif