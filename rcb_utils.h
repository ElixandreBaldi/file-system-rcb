#ifndef RCB_FILE_SYSTEM_FAT_UTILS_H
#define RCB_FILE_SYSTEM_FAT_UTILS_H

#include <stdio.h>
#include "data_structures.h"

rcb_table rcb_tbl;

unsigned int free_positions(unsigned short sectors_per_disk) {
    unsigned int count = 0;
    for (unsigned short i = 0; i < sectors_per_disk; i++) {
        if (rcb_tbl.content[i] == EMPTY_SPACE) {
            count++;
        }
    }
    return count;
}

unsigned int read_rcb(FILE *device, unsigned short bytes_per_sector) {
    fseek(device, bytes_per_sector, SEEK_SET);
    fread(&rcb_tbl, 1, sizeof(rcb_tbl), device);
}

unsigned short *get_free_spaces(unsigned int sectors_needed, unsigned short sectors_per_disk) {
    unsigned short *values = malloc(sizeof(unsigned short) * sectors_needed);
    unsigned short j = 0;
    unsigned short i = 0;
    while (i < sectors_per_disk) {
        if (rcb_tbl.content[i] == EMPTY_SPACE) {
            values[j] = i;
            j++;
        }
        if (j == sectors_needed) {
            break;
        }
        i++;
    }
    return values;
}

void write_pos(unsigned short pos, unsigned short value) {
    rcb_tbl.content[pos] = value;
}

void sync_rcb(FILE *device, unsigned short bytes_per_sector) {
    fseek(device, bytes_per_sector, SEEK_SET);
    fwrite(&rcb_tbl, 1, sizeof(rcb_tbl), device);
}

unsigned short navigate(unsigned int value, FILE *device){
    unsigned short position;
    fseek(device, value , SEEK_SET);
    fread(&position, SPACE_SIZE, 1, device);
    return position;
}

#endif
