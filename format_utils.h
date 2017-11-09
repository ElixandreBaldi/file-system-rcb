#ifndef RCB_FILE_SYSTEM_FORMAT_UTILS_H
#define RCB_FILE_SYSTEM_FORMAT_UTILS_H

#include <stdio.h>
#include <errno.h>
#include "messages.h"
#include "boot_record_utils.h"
#include "rcb_allocation_table.h"

int hard_format(const char *device_name, int size) {
    FILE *device;
    boot_record btr;
    rcb_allocation_table rcb;
    int nil;
    int zero = 0;
    size_t read;
    device = fopen(device_name, "rb");
    if(device == NULL){
        print_invalid_device(strerror(errno));
        return 1;
    }
    fseek(device, 0, SEEK_SET);
    while ((read = fread(&nil, 1, sizeof(nil), device)) == sizeof(nil)) {
        fseek(device, -read, SEEK_CUR);
        fwrite(&zero, 1, read, device);
    }

    btr.rbc = (unsigned short) "RCB";
    btr.bytes_per_sector = (unsigned short) size;
    btr.sectors_per_rcb = (unsigned short) (131072 / btr.bytes_per_sector);
    btr.reserved_sectors = (unsigned short) (1 + btr.sectors_per_rcb);
    btr.entry_directory = 512; //TODO
    btr.sectors_per_disk = btr.bytes_per_sector * btr.sectors_per_rcb;

    return 0;
}

int soft_format(const char *device_name, int size) {
//    FILE *device;
//    device = fopen(device_name, "rb");
//    if(device == NULL){
//        return 1;
//    }
//
//    return 0;
}

#endif
