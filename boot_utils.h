#ifndef RCB_FILE_SYSTEM_BOOT_UTILS_H
#define RCB_FILE_SYSTEM_BOOT_UTILS_H

#include <stdio.h>
#include "data_structures.h"

boot_record read_boot_record(boot_record boot, FILE *device) {
    fread(&boot, 1, sizeof(boot), device);
    return boot;
}

bool is_valid_boot_record(boot_record boot) {
    char signature[2] = SIGNATURE;
    for(int i = 0; i < 2; i++) {
        if (signature[i]!= boot.rcb[i]) {
            return false;
        }
    }
    return boot.sectors_per_rcb && boot.bytes_per_partition && boot.entry_directory &&
           boot.reserved_sectors && boot.sectors_per_disk && boot.bytes_per_sector;
}

#endif
