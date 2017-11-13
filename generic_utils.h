#ifndef RCB_FILE_SYSTEM_GENERIC_UTILS_H
#define RCB_FILE_SYSTEM_GENERIC_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define RCB_TABLE_SIZE 131072
#define RCB_DATA_TABLE 32
#define SIGNATURE "RCB!"

long get_device_size(FILE *device) {
    long size;
    fseek(device, 0L, SEEK_END);
    size = ftell(device);
    rewind(device);

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
