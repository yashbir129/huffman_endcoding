#ifndef CORE_H
#define CORE_H

<<<<<<< HEAD
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 256

// Huffman tree node
typedef struct Node {
    unsigned char ch;
    uint64_t freq;
    struct Node *left, *right;
} Node;

// Min-heap (priority queue)
typedef struct MinHeap {
    int size;
    int capacity;
    Node **arr;
} MinHeap;

// Code table (string representation)
typedef struct {
    char *bits; // dynamically allocated bitstring (e.g., "1010")
} Code;

Node* create_node(unsigned char ch, uint64_t freq);
MinHeap* create_minheap(int capacity);
void minheap_insert(MinHeap *h, Node *node);
Node* minheap_extract(MinHeap *h);
void minheap_free(MinHeap *h);
Node* build_huffman_tree(uint64_t freq_table[]);
void build_code_table(Node *root, Code code_table[], char *buffer, int depth);
void free_huffman_tree(Node *root);
void write_uint64(FILE *f, uint64_t v);
uint64_t read_uint64(FILE *f);

#endif
=======
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
>>>>>>> f98685467c920b6d7d448c00b85ec85a17a3aa9b
