#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Define necessary constants and structures from core.h
#define MAX_CHARS 256

typedef struct Node {
    unsigned char ch;
    uint64_t freq;
    struct Node *left, *right;
} Node;

typedef struct {
    char *bits;
} Code;

// Function prototypes
Node* build_huffman_tree(uint64_t freq_table[]);
void free_huffman_tree(Node *root);
void build_code_table(Node *root, Code code_table[], char *buffer, int depth);

// Simple function to build frequency table from a string
void build_frequency_table(const char *text, uint64_t freq_table[]) {
    memset(freq_table, 0, MAX_CHARS * sizeof(uint64_t));
    
    for (int i = 0; text[i] != '\0'; i++) {
        freq_table[(unsigned char)text[i]]++;
    }
}

// Simple function to print the Huffman codes
void print_codes(Code code_table[]) {
    printf("Huffman Codes:\n");
    for (int i = 0; i < MAX_CHARS; i++) {
        if (code_table[i].bits != NULL) {
            if (i >= 32 && i <= 126) { // Printable ASCII
                printf("'%c': %s\n", i, code_table[i].bits);
            } else {
                printf("%d: %s\n", i, code_table[i].bits);
            }
        }
    }
}

// Implementation of Huffman tree building
Node* build_huffman_tree(uint64_t freq_table[]) {
    // Count non-zero frequencies
    int count = 0;
    for (int i = 0; i < MAX_CHARS; i++) {
        if (freq_table[i] > 0) count++;
    }
    
    if (count == 0) return NULL;
    
    // Create nodes for each character
    Node **nodes = (Node**)malloc(count * sizeof(Node*));
    int idx = 0;
    for (int i = 0; i < MAX_CHARS; i++) {
        if (freq_table[i] > 0) {
            nodes[idx] = (Node*)malloc(sizeof(Node));
            nodes[idx]->ch = (unsigned char)i;
            nodes[idx]->freq = freq_table[i];
            nodes[idx]->left = nodes[idx]->right = NULL;
            idx++;
        }
    }
    
    // Build the tree by combining nodes
    while (count > 1) {
        // Find two nodes with lowest frequencies
        int min1 = 0, min2 = 1;
        if (nodes[min1]->freq > nodes[min2]->freq) {
            int temp = min1;
            min1 = min2;
            min2 = temp;
        }
        
        for (int i = 2; i < count; i++) {
            if (nodes[i]->freq < nodes[min1]->freq) {
                min2 = min1;
                min1 = i;
            } else if (nodes[i]->freq < nodes[min2]->freq) {
                min2 = i;
            }
        }
        
        // Create a new internal node
        Node *internal = (Node*)malloc(sizeof(Node));
        internal->ch = 0;
        internal->freq = nodes[min1]->freq + nodes[min2]->freq;
        internal->left = nodes[min1];
        internal->right = nodes[min2];
        
        // Replace the first minimum with the new node
        nodes[min1] = internal;
        
        // Remove the second minimum by shifting
        nodes[min2] = nodes[count - 1];
        count--;
    }
    
    Node *root = nodes[0];
    free(nodes);
    return root;
}

// Free the Huffman tree
void free_huffman_tree(Node *root) {
    if (root) {
        free_huffman_tree(root->left);
        free_huffman_tree(root->right);
        free(root);
    }
}

// Build the code table
void build_code_table(Node *root, Code code_table[], char *buffer, int depth) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        buffer[depth] = '\0';
        code_table[root->ch].bits = strdup(buffer);
        return;
    }
    
    if (root->left) {
        buffer[depth] = '0';
        build_code_table(root->left, code_table, buffer, depth + 1);
    }
    
    if (root->right) {
        buffer[depth] = '1';
        build_code_table(root->right, code_table, buffer, depth + 1);
    }
}

int main() {
    const char *sample_text = "hello world! this is a test of huffman encoding.";
    printf("Original text: %s\n", sample_text);
    
    // Build frequency table
    uint64_t freq_table[MAX_CHARS];
    build_frequency_table(sample_text, freq_table);
    
    // Print character frequencies
    printf("\nCharacter frequencies:\n");
    for (int i = 0; i < MAX_CHARS; i++) {
        if (freq_table[i] > 0) {
            if (i >= 32 && i <= 126) { // Printable ASCII
                printf("'%c': %llu\n", i, (unsigned long long)freq_table[i]);
            } else {
                printf("%d: %llu\n", i, (unsigned long long)freq_table[i]);
            }
        }
    }
    
    // Build Huffman tree
    Node *root = build_huffman_tree(freq_table);
    if (!root) {
        printf("Failed to build Huffman tree.\n");
        return 1;
    }
    
    // Build code table
    Code code_table[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; i++) {
        code_table[i].bits = NULL;
    }
    
    char buffer[MAX_CHARS];
    build_code_table(root, code_table, buffer, 0);
    
    // Print codes
    print_codes(code_table);
    
    // Calculate compression ratio
    int original_size = strlen(sample_text) * 8; // 8 bits per character
    int compressed_size = 0;
    
    for (int i = 0; sample_text[i] != '\0'; i++) {
        unsigned char ch = sample_text[i];
        if (code_table[ch].bits) {
            compressed_size += strlen(code_table[ch].bits);
        }
    }
    
    printf("\nOriginal size: %d bits\n", original_size);
    printf("Compressed size: %d bits\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", 100.0 * (original_size - compressed_size) / original_size);
    
    // Clean up
    for (int i = 0; i < MAX_CHARS; i++) {
        if (code_table[i].bits) {
            free(code_table[i].bits);
        }
    }
    free_huffman_tree(root);
    
    return 0;
}