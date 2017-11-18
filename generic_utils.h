#ifndef RCB_FILE_SYSTEM_GENERIC_UTILS_H
#define RCB_FILE_SYSTEM_GENERIC_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "data_structures.h"

#define RCB_TABLE_SIZE 65536
#define RCB_DATA_TABLE 32
#define SIGNATURE ":3"
#define EMPTY_SPACE 0xFFFE
#define RCB_EOF 0xFFFF
#define SPACE_SIZE 2
#define BTR_SIZE 16
#define DIR_ENTRY 256 // decisao do gerente

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
    if (sect_size < BTR_SIZE) {
        print_insufficient_sect_size(BTR_SIZE);
        return BTR_SIZE;
    }
    if (!is_power_of_2(sect_size)) {
        unsigned int res;
        res = (unsigned int) pow(2, floor(log2(sect_size)));
        print_sect_size_not_power_of_two(res);
        return res;
    }

    return sect_size;
}

#endif
