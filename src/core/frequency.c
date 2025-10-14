#include "core.h"
<<<<<<< HEAD
#include <string.h>

// Build frequency table from a file
int build_frequency_table_from_file(const char *path, uint64_t freq_table[]) {
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    memset(freq_table, 0, sizeof(uint64_t) * MAX_CHARS);
    int c;
    while ((c = fgetc(f)) != EOF) {
        freq_table[(unsigned char)c]++;
    }
    fclose(f);
    return 0;
}
=======

void get_frequency(const char *filename, int freq[]) {
    for (int i = 0; i < 256; i++) freq[i] = 0;
    
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File open failed");
        return;
    }
    int c;
    while ((c = fgetc(fp)) != EOF)
        freq[c]++;
    fclose(fp);
}
>>>>>>> f98685467c920b6d7d448c00b85ec85a17a3aa9b
