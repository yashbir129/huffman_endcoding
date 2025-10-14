#include "core.h"
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
