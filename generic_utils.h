#ifndef RCB_FILE_SYSTEM_GENERIC_UTILS_H
#define RCB_FILE_SYSTEM_GENERIC_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define RCB_TABLE_SIZE 65536
#define RCB_DATA_TABLE 32
#define SIGNATURE ":3"
#define EMPTY_SPACE 0xFFFE
#define SPACE_SIZE 2

unsigned long get_size(FILE *stream) {
    unsigned long size;
    fseek(stream, 0L, SEEK_END);
    size = (unsigned long) ftell(stream);
    rewind(stream);

    return size;
}

bool is_power_of_2(unsigned int x) {
    return ((x != 0) && !(x & (x - 1)));
}

unsigned int parse_sect(unsigned int sect_size) {
    if (!is_power_of_2(sect_size)) {
        unsigned int res;
        printf("Warning! The sector size is not a power of two and will be decreased.\n");
        res = (unsigned int) pow(2, floor(log2(sect_size)));
        printf("Actual sector size: %d\n", res);
        return res;
    }

    return sect_size;
}

#endif
