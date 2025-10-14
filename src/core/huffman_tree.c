#include "core.h"
<<<<<<< HEAD
#include <string.h>
#include <stdint.h>

// strdup may be unavailable on some platforms; provide a local helper
static char *str_dup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}

// Helper forward declarations
static void minheapify(MinHeap *h, int idx);
static Node* new_node(unsigned char ch, uint64_t freq);

Node* create_node(unsigned char ch, uint64_t freq) {
    return new_node(ch, freq);
}

static Node* new_node(unsigned char ch, uint64_t freq) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->ch = ch;
    n->freq = freq;
    n->left = n->right = NULL;
    return n;
}

MinHeap* create_minheap(int capacity) {
    MinHeap *h = (MinHeap*)malloc(sizeof(MinHeap));
    h->size = 0;
    h->capacity = capacity;
    h->arr = (Node**)malloc(sizeof(Node*) * capacity);
    return h;
}

static void swap_nodes(Node **a, Node **b) {
    Node *t = *a; *a = *b; *b = t;
}

void minheap_insert(MinHeap *h, Node *node) {
    if (h->size == h->capacity) {
        // reallocate
        h->capacity *= 2;
        h->arr = (Node**)realloc(h->arr, h->capacity * sizeof(Node*));
    }
    int i = h->size++;
    h->arr[i] = node;
    // bubble up
    while (i && h->arr[i]->freq < h->arr[(i-1)/2]->freq) {
        swap_nodes(&h->arr[i], &h->arr[(i-1)/2]);
        i = (i-1)/2;
    }
}

Node* minheap_extract(MinHeap *h) {
    if (h->size == 0) return NULL;
    Node *root = h->arr[0];
    h->arr[0] = h->arr[--h->size];
    minheapify(h, 0);
    return root;
}

static void minheapify(MinHeap *h, int idx) {
    int smallest = idx;
    int l = 2*idx + 1;
    int r = 2*idx + 2;
    if (l < h->size && h->arr[l]->freq < h->arr[smallest]->freq) smallest = l;
    if (r < h->size && h->arr[r]->freq < h->arr[smallest]->freq) smallest = r;
    if (smallest != idx) {
        swap_nodes(&h->arr[smallest], &h->arr[idx]);
        minheapify(h, smallest);
    }
}

void minheap_free(MinHeap *h) {
    free(h->arr);
    free(h);
}

Node* build_huffman_tree(uint64_t freq_table[]) {
    // count unique
    int i, uniq = 0;
    for (i = 0; i < MAX_CHARS; ++i) if (freq_table[i]) uniq++;
    if (uniq == 0) return NULL;
    MinHeap *h = create_minheap(uniq + 4);
    for (i = 0; i < MAX_CHARS; ++i) {
        if (freq_table[i]) {
            Node *n = create_node((unsigned char)i, freq_table[i]);
            minheap_insert(h, n);
        }
    }

    while (h->size > 1) {
        Node *l = minheap_extract(h);
        Node *r = minheap_extract(h);
        Node *parent = create_node(0, l->freq + r->freq);
        parent->left = l; parent->right = r;
        minheap_insert(h, parent);
    }
    Node *root = minheap_extract(h);
    minheap_free(h);
    return root;
}

void build_code_table(Node *root, Code code_table[], char *buffer, int depth) {
    if (!root) return;
    if (!root->left && !root->right) {
        buffer[depth] = '\0';
    code_table[root->ch].bits = str_dup(buffer);
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

void free_huffman_tree(Node *root) {
    if (!root) return;
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}

void write_uint64(FILE *f, uint64_t v) {
    // Write in little-endian
    for (int i = 0; i < 8; ++i) {
        uint8_t b = (v >> (i*8)) & 0xFF;
        fputc(b, f);
    }
}

uint64_t read_uint64(FILE *f) {
    uint64_t v = 0;
    for (int i = 0; i < 8; ++i) {
        int b = fgetc(f);
        if (b == EOF) return (uint64_t)-1;
        v |= ((uint64_t)(uint8_t)b) << (i*8);
    }
    return v;
}
=======

Node* create_node(unsigned char ch, int freq, Node *left, Node *right) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;
    return node;
}

Node* build_huffman_tree(int freq[]) {
    // Normally you'd use a priority queue here.
    // For demo: return a dummy node.
    Node *root = create_node('*', 0, NULL, NULL);
    return root;
}

void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}
>>>>>>> f98685467c920b6d7d448c00b85ec85a17a3aa9b
