#include "core.h"
#include <string.h>
#include <stdio.h>

// Simple wrappers / prototypes used by encoder/decoder

// Save frequency table header:
// Format:
// 8 bytes: original file size (uint64)
// 4 bytes: number of unique symbols (uint32)
// For each symbol:
//   1 byte: symbol
//   8 bytes: freq (uint64)
int write_header_with_freq(FILE *out, uint64_t original_size, uint64_t freq_table[]) {
    // write original size
    write_uint64(out, original_size);
    // count unique
    uint32_t uniq = 0;
    for (int i = 0; i < MAX_CHARS; ++i) if (freq_table[i]) uniq++;
    // write uniq as 4 bytes little endian
    uint32_t x = uniq;
    for (int i = 0; i < 4; ++i) fputc((x >> (i*8)) & 0xFF, out);
    for (int i = 0; i < MAX_CHARS; ++i) {
        if (freq_table[i]) {
            fputc((unsigned char)i, out);
            write_uint64(out, freq_table[i]);
        }
    }
    return 0;
}

// Read header; fills freq_table and returns original_size. On error returns (uint64_t)-1
uint64_t read_header_and_freq(FILE *in, uint64_t freq_table[]) {
    for (int i = 0; i < MAX_CHARS; ++i) freq_table[i] = 0;
    uint64_t original_size = read_uint64(in);
    if (original_size == (uint64_t)-1) return (uint64_t)-1;
    // read uniq
    uint32_t uniq = 0;
    int b;
    for (int i = 0; i < 4; ++i) {
        b = fgetc(in);
        if (b == EOF) return (uint64_t)-1;
        uniq |= ((uint32_t)(uint8_t)b) << (i*8);
    }
    for (uint32_t k = 0; k < uniq; ++k) {
        int ch = fgetc(in);
        if (ch == EOF) return (uint64_t)-1;
        uint64_t freq = read_uint64(in);
        freq_table[(unsigned char)ch] = freq;
    }
    return original_size;
}
