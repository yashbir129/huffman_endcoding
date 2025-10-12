#include "core.h"

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