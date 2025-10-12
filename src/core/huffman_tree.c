#include "core.h"

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