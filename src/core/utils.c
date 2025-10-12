#include "core.h"

void print_freq_table(int freq[]) {
    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
            printf("%c : %d\n", i, freq[i]);
}