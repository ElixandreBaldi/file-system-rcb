#ifndef RCB_FILE_SYSTEM_FAT_UTILS_H
#define RCB_FILE_SYSTEM_FAT_UTILS_H

#include <stdio.h>
#include "data_structures.h"

rcb_table rcb_tbl;

unsigned int free_positions () {
    unsigned int i;
    unsigned int count = 0;
    for (i = 0; i < RCB_TABLE_SIZE; i++) {
        if (rcb_tbl.content[i] == EMPTY_SPACE) {
            count++;
        }
    }
    return count;
}

unsigned int read_rcb (FILE *device, unsigned short bytes_per_sector) {
    fseek(device, bytes_per_sector, SEEK_SET);
    fread(&rcb_tbl, 1, sizeof(rcb_tbl), device);
}

#endif
